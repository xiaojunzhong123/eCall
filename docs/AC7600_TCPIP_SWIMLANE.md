# AC7600 TCP/IP 主路径泳道图

## 目的和当前证据

本文件是 TBOX eCall 样机 AC7600 TCP/IP 主路径的可编辑 Mermaid 源文件。

- 原厂资料：`D:\TBOX\资料\小华半导体\08-4G模组\A7600C R2&A7608X-H&A7602X-H\01 APP手册\A76XX Series_TCPIP_Application Note_V1.05.pdf`。
- AT 语法资料：`D:\TBOX\资料\小华半导体\08-4G模组\A7600C R2&A7608X-H&A7602X-H\03 ATC手册\A76XX_Series_AT_Command_Manual_V2.02.pdf`。
- 实板已验证：HC32 UART4 115200 bit/s 经 CH340 临时桥接 AC7600 的 1.8 V TX；已确认 TCP 建连、平台注册 `register_ack`、心跳 `heartbeat_ack`、MSD 上传和 `msd_ack`。
- 未验证不标为完成：真实 `ATD` 外呼、通话 URC 解析与拨号失败重试。

## 总览

```mermaid
flowchart LR
    A[KL15 ON / eCall 请求] --> B[阶段 1：基础入网]
    B --> C[阶段 2：PDP 激活与地址确认]
    C --> D[阶段 3：TCP 配置与建连]
    D --> E[阶段 4：平台注册与心跳]
    E --> F[阶段 5：MSD 上传]
    F --> G[阶段 6：ATD 外呼与重试]

    B -.实板已验证.-> C
    C -.实板已验证.-> D
    D -.实板已验证.-> E
    E -.实板已验证.-> F
    F -.MSD/ATD/远端振铃已实板观察；接通与重试待验证.-> G
```

## 第 4 阶段软件分层收口

```text
ModemTask
  -> ModemService       网络启动、平台活动、休眠和通话的顶层编排
       -> ModemNetwork  AT 同步、PDP 与 TCP 建连
       -> ModemPlatform 注册、在线收发、心跳与 MSD 上传的私有状态机
       -> ModemCall     独立通话状态机
ModemNetwork / ModemPlatform / ModemCall
  -> ModemPort          单笔 AT 事务、响应、URC 与超时
```

`ModemService` 只维护 `SLEEP -> NETWORK_STARTUP -> PLATFORM_ACTIVE` 三个业务大阶段；AT 同步、PDP 与 TCP 的细阶段
完全由 `ModemNetwork` 私有维护。TCP 建连后，`ModemPlatform` 私有维护
`REGISTERING -> ONLINE -> MSD_UPLOADING -> ONLINE`；服务层只用稳定在线查询决定休眠，
并且只把平台失败事件提升为顶层状态变化。

KL15 延迟休眠只在平台处理前已经在线、且本周期处理后仍在线时累计。因此注册完成或 MSD 上传完成而刚进入
`ONLINE` 的同一调度周期不会提前累计休眠时间，下一周期才开始执行在线休眠策略。

## 阶段 1：基础入网（实板已验证）

```mermaid
sequenceDiagram
    participant T as ModemTask
    participant S as ModemService
    participant P as ModemPort
    participant M as AC7600

    T->>S: KL15_ON / 请求联网
    S->>P: AT（串口响应测试）
    P->>M: AT
    M-->>P: OK

    S->>P: ATE0（关闭命令回显）
    P->>M: ATE0
    M-->>P: OK

    S->>P: AT+CPIN?（查询 SIM 状态）
    P->>M: AT+CPIN?
    M-->>P: +CPIN: READY
    M-->>P: OK

    S->>P: AT+CSQ（查询信号质量）
    P->>M: AT+CSQ
    M-->>P: +CSQ: rssi,ber
    M-->>P: OK

    S->>P: AT+CEREG?（查询 LTE 注册）
    P->>M: AT+CEREG?
    M-->>P: +CEREG: n,1 或 n,5
    M-->>P: OK
    S->>S: NETWORK_REGISTERED
```

`+CEREG` 的注册状态为 `1`（本地）或 `5`（漫游）才进入 PDP 阶段；单独收到 `OK` 不代表已注册。

## 阶段 2：PDP 激活与地址确认（实板已验证）

```mermaid
sequenceDiagram
    participant S as ModemService
    participant P as ModemPort
    participant M as AC7600

    S->>P: AT+NETOPEN?（查询 PDP 是否已打开）
    P->>M: AT+NETOPEN?
    M-->>P: +NETOPEN: 1 或 0
    M-->>P: OK

    alt PDP 已打开
        S->>P: AT+NETCLOSE（关闭残留 PDP 与 socket）
        P->>M: AT+NETCLOSE
        M-->>P: OK
        M-->>P: +NETCLOSE: 0
        S->>S: PDP_CLOSED
    else PDP 未打开
    end

    S->>P: AT+NETOPEN（激活干净 PDP）
    P->>M: AT+NETOPEN
    M-->>P: OK
    M-->>P: +NETOPEN: 0
    S->>S: PDP_OPENED

    S->>P: AT+IPADDR（读取 PDP 地址）
    P->>M: AT+IPADDR
    M-->>P: IPv6 地址
    M-->>P: IPv4 地址
    M-->>P: OK
    S->>S: PDP_IPADDR_READY / PDP_ACTIVE
```

实板已观察到 IPv6 与 IPv4 双栈地址。每次新网络流程若发现 PDP 已打开，会先执行 `NETCLOSE` 清除残留 socket/PDP，再重新 `NETOPEN`；`NETOPEN` 的成功条件是异步结果 `+NETOPEN: 0`，而不是先返回的 `OK`。不使用 `CRESET`，仅在连续软恢复失败时再考虑模组复位。

## 阶段 3：TCP 配置与建连（实板已验证）

```mermaid
sequenceDiagram
    participant S as ModemService
    participant P as ModemPort
    participant M as AC7600
    participant V as 平台服务器

    S->>S: PDP_ACTIVE
    S->>P: AT+CTCPKA=1,2,5,1（开启 TCP 保活）
    P->>M: AT+CTCPKA=1,2,5,1
    M-->>P: OK
    S->>S: TCP_KEEPALIVE_READY

    S->>P: AT+CIPCFG="DATAECHO",OFF（关闭 CIPSEND 数据回显）
    P->>M: AT+CIPCFG="DATAECHO",OFF
    M-->>P: OK
    S->>S: TCP_CFG_READY

    S->>P: AT+CIPOPEN=0,"TCP","8.137.175.163",8902
    P->>M: AT+CIPOPEN=0,"TCP","8.137.175.163",8902
    M-->>P: OK（命令已受理）
    M->>V: TCP 建连
    M-->>P: +CIPOPEN: 0,0（链路 0 建连成功）
    P-->>S: 匹配任意 +CIPOPEN: 后判定结果码
    S->>S: TCP_CONNECTED / PLATFORM_REGISTERING
```

`AT+CTCPKA=1,2,5,1` 的含义是：启用保活、空闲 2 分钟后探测、最多重试 5 次、探测间隔 1 分钟。`CIPOPEN` 必须等待 `+CIPOPEN: 0,0`；`+CIPOPEN: 0,<非零>` 会立即记录 `TCP_OPEN_RESULT_FAILED` 并进入休眠，避免误判或长时间等待。

当前实现对 `CIPOPEN` 失败增加了 3 次受控重试，间隔 5 秒；保留已完成的 PDP 和 TCP 配置，耗尽后才进入 `SLEEP`。

当前服务器 `8.137.175.163:8902` 是从 AECS 参考工程沿用的候选平台，已于上述样机试验中实际连接成功；其平台协议尚未冻结。

## 阶段 4：平台注册、发送与接收（实板已验证）

```mermaid
sequenceDiagram
    participant S as ModemService
    participant P as ModemPort
    participant M as AC7600
    participant V as 平台服务器

    S->>P: AT+CIPSEND=0,<length>（申请发送固定长度注册帧）
    P->>M: AT+CIPSEND=0,<length>
    M-->>P: >（允许写入负载）
    P->>M: <length> 字节原始注册帧
    M-->>P: OK
    M-->>P: +CIPSEND: 0,<req>,<cnf>
    P-->>S: 仅当 req == cnf 视为发送完成
    M-->>V: 注册数据

    V-->>M: 注册应答 / 平台数据
    M-->>P: +CIPRXGET: 1,0（存在缓存数据）
    S->>P: AT+CIPRXGET=4,0（查询未读长度）
    S->>P: AT+CIPRXGET=3,0,<readLength>（读取十六进制数据）
    M-->>P: +CIPRXGET: 3,0,<readLength>,0 + 十六进制负载 + OK
    P-->>S: 还原字节并校验 AECS 类型/长度帧头
    S->>S: 有界提取 JSON "type" 并记录语义线索
```

当前源码已实现候选注册发送：`01 + 4 字节大端 JSON 长度 + {"role":"device","deviceId":"7091"}`，经 `CIPSEND` 的 `>` 提示符发送，并以 `+CIPSEND: 0,req,cnf` 且 `req == cnf` 判定成功。TCP 建连前会先执行 `AT+CIPRXGET=1`，平台下行只上报 `+CIPRXGET: 1,<link>`；随后查询缓存长度，并对不超过 750 字节的数据使用 `AT+CIPRXGET=3,<link>,<length>` 读回十六进制文本、还原字节并校验候选 AECS 帧头。

在帧头校验通过后，服务仅在帧内有界扫描 JSON 的 `"type":"..."`，最多记录 31 个可打印 ASCII 字符为 `PLATFORM_RX_TYPE:<value>`；不打印完整平台 JSON，也不因未知 `type` 改写当前 `ONLINE` 状态。实板已收到长度 127 的候选 AECS 帧，并确认其 `type` 为 `register_ack`；该回包的附加成功/失败字段语义仍未冻结。

### ONLINE 内的候选心跳（实板已验证）

```mermaid
sequenceDiagram
    participant S as ModemService
    participant P as ModemPort
    participant M as AC7600
    participant V as 平台服务器

    S->>S: ONLINE 空闲累计 25 s
    S->>P: AT+CIPSEND=0,<heartbeatLength>
    P->>M: AT+CIPSEND=0,<heartbeatLength>
    M-->>P: >
    P->>M: 02 + 长度 + {"type":"heartbeat","deviceId":"7091"}
    M-->>P: OK + +CIPSEND: 0,<req>,<cnf>
    P-->>S: req == cnf
    S->>S: HEARTBEAT_SENT，重新累计 25 s
    M->>V: TCP 心跳帧
```

心跳仅在平台接收空闲、没有待上传 MSD、且未请求 KL15 休眠时启动，因此不会与 `CIPRXGET` 或紧急业务竞争 UART AT 事务。CIPSEND 的提示符和长度确认与注册发送完全一致；任一步失败进入现有网络恢复路径。心跳帧内容、25 秒周期仍是 AECS 候选，需看服务器响应后再冻结。

## 阶段 5：MSD、外呼与故障恢复（MSD 实板已验证）

```mermaid
sequenceDiagram
    participant E as EcallTask
    participant S as ModemService
    participant P as ModemPort
    participant M as AC7600

    E->>S: 冻结 MSD，申请紧急服务
    S->>P: AT+CIPSEND=0,116（申请固定长度 MSD 帧）
    M-->>P: >
    P->>M: 10 + 111 字节 MSD 候选负载
    M-->>P: OK + +CIPSEND: 0,116,116
    S->>S: MSD_UPLOADED_CALL_PENDING
    S->>P: ATD<configuredNumber>;
    M-->>P: OK（命令已受理）
    M-->>P: +CLCC: ...,2/3,...（拨号中/远端振铃）
    M-->>P: VOICE CALL: BEGIN 或 +CLCC: ...,0,...（接通）
    M-->>P: VOICE CALL: END / NO CARRIER / +CLCC: ...,6,...（结束或失败）
    S-->>E: 呼叫状态事件

    M-->>P: +CIPEVENT: NETWORK CLOSED UNEXPECTEDLY
    S->>P: CIPCLOSE / NETOPEN? / NETCLOSE
    S->>S: 受控重连或 eCall 失败重试

    M-->>P: +CGEV: ... PDN DEACT ...
    S->>S: 终止当前 AT 事务，AT_SYNC → NETCLOSE/NETOPEN 重建
```

当前 MSD 使用 AECS 参考工程的 111 字节负载布局：`version`、`identifier`、触发掩码、测试呼叫、车辆类型、17 字节 VIN、动力类型、UTC、经度、纬度、9 组历史点、Delta-V、乘员和翻滚。当前 TBOX 已有的触发掩码、UTC、经纬度会映射进帧；演示常量使用版本 `1`、标识 `1`、测试呼叫 `1` 与 VIN `LDC12345678900001`，其余当前没有来源的字段显式填零。外层帧使用消息类型 `0x10` 和 4 字节大端长度，因此 CIPSEND 总长度为 116 字节。

`+CIPSEND: 0,116,116` 证明模组已完整交付 MSD 给 TCP 栈；实板随后通过 `CIPRXGET=4/3` 读取 121 字节平台应答，解析到 `msd_ack`。紧急请求若撞上在途的心跳或下行读取，会等待该单笔事务结束后才进入 MSD，避免并发 AT；在已授权号码与时机下，`msd_ack` 后会发送 `ATD`。最近实板日志已观察到该号码的 `+CLCC` 状态 `3`（远端振铃）；本次由被叫未接听而未进入接通状态，随后收到状态 `6`、`VOICE CALL: END` 与 `NO CARRIER`。这验证了拨号和振铃链路，但接通场景仍待验证。`+CLCC` 状态 `2/3` 会停止本地 30 秒误超时，等待终态 URC；自动重试仍未实现。

## 当前完成度与下一步

| 项目 | 状态 | 实际证据 / 约束 |
|---|---|---|
| UART4 AT 收发 | 已验证 | AC7600 对基础 AT 和 TCP AT 均有实机响应。|
| LTE 注册 | 已验证 | `+CEREG: 0,1`。|
| PDP | 已验证 | `NETOPEN?`、`NETOPEN`、`IPADDR` 已完成；观察到 IPv4/IPv6 地址。|
| TCP | 已验证 | `CTCPKA`、`CIPCFG`、`CIPOPEN`；收到 `+CIPOPEN: 0,0`。|
| RTT 日志 | 已调整 | HMI 队列 32 条、RTT 上行缓冲 4 KiB；仍需长时间无断点观察。|
| 平台注册 | 已实板验证 | 注册帧发送完成后，实测收到 `register_ack`；附加字段语义尚未冻结。|
| CIPSEND | 已实板验证 | 已实测 `>`、固定长度原始数据与 `+CIPSEND` 长度确认。|
| 接收事件/长度 | 已实板验证 | 已实测 `CIPRXGET=1` 缓冲下行与 `CIPRXGET=4` 长度查询。|
| 接收内容/帧头 | 已实板验证 | 使用 `CIPRXGET=3` 读取最多 750 字节十六进制内容；已收到并通过 127 字节候选 AECS 类型/长度帧头校验。|
| 平台应答 type | 已实板验证 | 帧头有效后有界提取 JSON `type`；实测注册回包为 `register_ack`，附加字段语义尚未冻结。|
| 平台心跳 | 已实板验证 | ONLINE 空闲 25 秒后发送候选 43 字节 `0x02` 心跳帧，连续收到 `heartbeat_ack`；与下行接收、MSD 和 KL15 休眠互斥。|
| MSD 上报 | 已实板验证 | 发送 116 字节候选 MSD 帧，实测 `+CIPSEND: 0,116,116` 后读取并解析到 121 字节 `msd_ack`。|
| ATD / 单次拨号 | 已实现，实板已振铃 | `msd_ack` 后已观察到 `+CLCC` 状态 `3`；本次被叫未接听，后续收到状态 `6`、`VOICE CALL: END` 与 `NO CARRIER`。|
| 拨号失败重试 | 待实现 | SRS 要求 10s/30s/120s、至少 60min 的独立语音重试；当前不自动重复外呼。|
