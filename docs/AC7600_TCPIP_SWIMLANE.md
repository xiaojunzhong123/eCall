# AC7600 TCP/IP 主路径泳道图

## 目的和当前证据

本文件是 TBOX eCall 样机 AC7600 TCP/IP 主路径的可编辑 Mermaid 源文件。

- 原厂资料：`D:\TBOX\资料\小华半导体\08-4G模组\A7600C R2&A7608X-H&A7602X-H\01 APP手册\A76XX Series_TCPIP_Application Note_V1.05.pdf`。
- AT 语法资料：`D:\TBOX\资料\小华半导体\08-4G模组\A7600C R2&A7608X-H&A7602X-H\03 ATC手册\A76XX_Series_AT_Command_Manual_V2.02.pdf`。
- 实板已验证：2026-08-27，HC32 UART4 115200 bit/s，经 CH340 临时单向回传模组的 1.8 V TX 数据至 MCU；日志已确认到 `+CIPOPEN: 0,0` 和 `TCP_CONNECTED`。
- 未验证不标为完成：平台注册、平台回包解析、`CIPSEND`、MSD 上传、真实 `ATD` 外呼与失败重试。

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
    D -.实板已验证至 TCP_CONNECTED.-> E
    E -.待实现.-> F
    F -.待实现.-> G
```

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
        S->>S: PDP_ALREADY_OPEN
    else PDP 未打开
        S->>P: AT+NETOPEN（激活 PDP）
        P->>M: AT+NETOPEN
        M-->>P: OK
        M-->>P: +NETOPEN: 0
        S->>S: PDP_OPENED
    end

    S->>P: AT+IPADDR（读取 PDP 地址）
    P->>M: AT+IPADDR
    M-->>P: IPv6 地址
    M-->>P: IPv4 地址
    M-->>P: OK
    S->>S: PDP_IPADDR_READY / PDP_ACTIVE
```

实板已观察到 IPv6 与 IPv4 双栈地址。`AT+NETOPEN` 的成功条件是异步结果 `+NETOPEN: 0`，而不是先返回的 `OK`。

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

当前服务器 `8.137.175.163:8902` 是从 AECS 参考工程沿用的候选平台，已于上述样机试验中实际连接成功；其平台协议尚未冻结。

## 阶段 4：平台注册、发送与接收（待实现）

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
    S->>P: AT+CIPRXGET=2,0,<readLength>（读取 ASCII 数据）
```

平台注册报文、回包语义、心跳内容与周期尚未确认；当前 `PLATFORM_REGISTERING` 仅为状态机入口，未发送数据。二进制负载需要十六进制读取或长度驱动接收，不能按普通 CR/LF 文本行解析。

## 阶段 5：MSD、外呼与故障恢复（待实现）

```mermaid
sequenceDiagram
    participant E as EcallTask
    participant S as ModemService
    participant P as ModemPort
    participant M as AC7600

    E->>S: 冻结 MSD，申请紧急服务
    S->>P: CIPSEND 命令 / 等待 > / 写 MSD 原始字节
    M-->>P: +CIPSEND: 0,<req>,<cnf>
    S->>P: ATD<emergencyNumber>;（号码与时机需授权）
    M-->>P: 呼叫结果 URC
    S-->>E: 呼叫状态 / 失败重试事件

    M-->>P: +CIPEVENT: NETWORK CLOSED UNEXPECTEDLY
    S->>P: CIPCLOSE / NETOPEN? / NETCLOSE
    S->>S: 受控重连或 eCall 失败重试
```

## 当前完成度与下一步

| 项目 | 状态 | 实际证据 / 约束 |
|---|---|---|
| UART4 AT 收发 | 已验证 | AC7600 对基础 AT 和 TCP AT 均有实机响应。|
| LTE 注册 | 已验证 | `+CEREG: 0,1`。|
| PDP | 已验证 | `NETOPEN?`、`NETOPEN`、`IPADDR` 已完成；观察到 IPv4/IPv6 地址。|
| TCP | 已验证 | `CTCPKA`、`CIPCFG`、`CIPOPEN`；收到 `+CIPOPEN: 0,0`。|
| RTT 日志 | 已调整 | HMI 队列 32 条、RTT 上行缓冲 4 KiB；仍需长时间无断点观察。|
| 平台注册 | 待实现 | 缺少已确认的注册帧和平台应答规则。|
| CIPSEND / 接收 | 待实现 | ModemPort 仍是行式 AT 事务，尚不支持 `>` 与二进制长度收发。|
| MSD / ATD / 重试 | 待实现 | 需要 MSD 格式、号码、呼叫 URC 和重试策略的明确授权。|

