# AC7600 TCP/IP 主路径泳道图

## 目的和依据

本文件是 TBOX eCall 样机的可编辑 TCP/IP 流程图源文件。图中使用 Mermaid，修改后可在支持 Mermaid 的 Markdown 预览中直接查看。

- 原厂依据：`4G_docs/A76XX Series_TCPIP_Application Note_V1.05.pdf`，第 7、10-11、15-17、25-27 页。
- 详细 AT 语法：`4G_docs/A76XX_Series_AT_Command_Manual_V2.02.pdf` 与 `4G_docs/AC7600_AT_REFERENCE.md`。
- 标记：`已实现` 表示当前源码已有流程；`待实现` 不代表已在样机验证；`待确认` 需要以当前 AC7600 实机响应为准。

## 总览

```mermaid
flowchart LR
    A[KL15 ON / eCall 请求] --> B[基础入网探测]
    B --> C[PDP 激活]
    C --> D[TCP 建连]
    D --> E[平台注册与心跳]
    E --> F[MSD 上传]
    F --> G[ATD 语音外呼]
    G --> H[通话结束或失败重试]

    B -.当前已实现.-> C
    C -.当前已实现至 PDP_ACTIVE.-> D
    D -.待实现.-> E
    E -.待实现.-> F
    F -.待实现.-> G
```

## 阶段 1：基础入网与 PDP 激活（当前主路径）

```mermaid
sequenceDiagram
    participant T as ModemTask
    participant S as ModemService
    participant P as ModemPort
    participant M as AC7600

    T->>S: KL15_ON / 请求联网
    S->>P: 已实现：AT（确认串口响应）
    P->>M: AT
    M-->>P: OK

    S->>P: 已实现：ATE0（关闭命令回显）
    P->>M: ATE0
    M-->>P: OK

    S->>P: 已实现：AT+CPIN?（SIM 是否 READY）
    P->>M: AT+CPIN?
    M-->>P: +CPIN: READY
    M-->>P: OK

    S->>P: 已实现：AT+CSQ（信号质量）
    P->>M: AT+CSQ
    M-->>P: +CSQ: rssi,ber
    M-->>P: OK

    S->>P: 已实现：AT+CEREG?（LTE 注册）
    P->>M: AT+CEREG?
    M-->>P: +CEREG: n,1 或 n,5
    M-->>P: OK

    S->>S: NETWORK_REGISTERED
    S->>P: 已实现：AT+NETOPEN（激活 PDP）
    P->>M: AT+NETOPEN
    M-->>P: OK
    M-->>P: +NETOPEN: 0
    S->>S: PDP_ACTIVE

    Note over S,M: 建议下一步加入 AT+IPADDR，读取 PDP 地址作为激活后的诊断证据。
```

成功条件不是单独的 `OK`：`AT+NETOPEN` 必须收到 `+NETOPEN: 0`。失败后应先用 `AT+NETOPEN?` 查询状态；应用说明建议仍无法激活时复位模组并重新检查 PDP 配置。

## 阶段 2：TCP 建连（待实现）

推荐使用 **非透传 + 缓冲接收模式**。原因是 eCall 平台数据可能为二进制帧；透明模式下不能执行 AT 命令，而默认直推模式会直接向串口输出 `+IPD<长度>` 和原始负载，当前按 CR/LF 收行的 ModemPort 无法可靠处理。

```mermaid
sequenceDiagram
    participant S as ModemService
    participant P as ModemPort
    participant M as AC7600
    participant V as 平台服务器

    S->>P: 待确认：AT+CTCPKA=...（TCP 内核心跳）
    P->>M: AT+CTCPKA=...
    M-->>P: OK

    S->>P: 待确认：AT+CIPCFG=...（CID / DATAECHO 等）
    P->>M: AT+CIPCFG=...
    M-->>P: OK

    S->>P: 推荐：AT+CIPRXGET=1（启用缓冲接收）
    P->>M: AT+CIPRXGET=1
    M-->>P: OK

    S->>P: AT+CIPOPEN=0,"TCP","<server>",<port>
    P->>M: AT+CIPOPEN=0,"TCP","<server>",<port>
    M-->>P: OK
    M-->>P: +CIPOPEN: 0,0
    S->>S: TCP_CONNECTED
    M-->>V: TCP 建连完成
```

`AT+CIPOPEN` 也必须等待 `+CIPOPEN: <link>,0`。`link=0` 是当前项目候选链路号；应用说明示例使用 `link=1`，两者在非透传模式下均属于合法范围，最终由项目配置统一冻结。

## 阶段 3：平台注册、发送与接收（待实现）

```mermaid
sequenceDiagram
    participant S as ModemService
    participant P as ModemPort
    participant M as AC7600
    participant V as 平台服务器

    S->>P: AT+CIPSEND=0,<length>
    P->>M: AT+CIPSEND=0,<length>
    M-->>P: >（允许写入固定长度负载）
    P->>M: <length> 字节原始注册/心跳/MSD负载
    M-->>P: OK
    M-->>P: +CIPSEND: 0,<req>,<cnf>
    P-->>S: 成功条件：req == cnf
    M-->>V: TCP 数据

    V-->>M: 平台应答
    M-->>P: +CIPRXGET: 1,0（有缓存数据）
    S->>P: AT+CIPRXGET=4,0（查询未读长度）
    P->>M: AT+CIPRXGET=4,0
    M-->>P: +CIPRXGET: 4,0,<length>
    S->>P: AT+CIPRXGET=2,0,<readLength>（ASCII读取）
    P->>M: AT+CIPRXGET=2,0,<readLength>
    M-->>P: +CIPRXGET: 2,0,<readLength>,<restLength>
    M-->>P: 平台负载
    M-->>P: OK
```

对于二进制平台负载，应使用应用说明定义的十六进制读取模式 `AT+CIPRXGET=3,<link>,<length>`，或扩展 ModemPort 的长度驱动二进制接收状态；不得把二进制负载当作普通 CR/LF 文本行解析。

## 阶段 4：MSD、外呼和故障恢复（待实现）

```mermaid
sequenceDiagram
    participant E as EcallTask
    participant S as ModemService
    participant P as ModemPort
    participant M as AC7600
    participant V as 平台服务器

    E->>S: 冻结 MSD，申请紧急服务
    S->>P: AT+CIPSEND=0,<msdLength>
    P->>M: CIPSEND 命令 / 等待 > / 写 MSD 原始字节
    M-->>P: +CIPSEND: 0,<req>,<cnf>
    M-->>V: MSD 数据
    S->>P: ATD<emergencyNumber>;（需明确授权）
    P->>M: ATD...;
    M-->>P: 呼叫结果 URC（待按 AC7600 实测定义）
    S-->>E: 呼叫状态 / 失败重试事件

    M-->>P: +CIPEVENT: NETWORK CLOSED UNEXPECTEDLY
    S->>P: AT+CIPCLOSE=<link>（按连接状态处理）
    S->>P: AT+NETOPEN?（确认 PDP 状态）
    S->>P: AT+NETCLOSE（若网络库仍打开）
    S->>S: 受控重连或 eCall 失败重试
```

## 与当前主路径的差异和后续顺序

|项|当前状态|应用说明要求/建议|下一步|
|---|---|---|---|
|基础 AT、SIM、CSQ、CEREG|已实现并实机观察响应|符合|保持。|
|`NETOPEN`|已实现，等待 `+NETOPEN: 0`|符合|实机验证耗时与失败码。|
|`IPADDR`|未实现|推荐作为 PDP 激活诊断|加入 PDP_ACTIVE 入口后的只读验证。|
|接收模式|未实现|默认直推；推荐 eCall 使用 `CIPRXGET=1` 缓冲模式|先实现缓冲模式和长度读取。|
|`CIPOPEN`|仅有框架|等待 `OK` 和 `+CIPOPEN:<link>,0`|确认服务器、端口、CID 后实施。|
|`CIPSEND`|仅有框架|两阶段：`>` 后定长原始字节，最终检查 `+CIPSEND`|扩展 ModemPort 事务类型。|
|被动断线|未实现|处理 `+CIPEVENT`，释放 socket/PDP 后受控重连|在 TCP 阶段实施。|
|MSD 与 `ATD`|未实现|先上传 MSD，再真实语音外呼|确认号码、平台确认规则、呼叫 URC 和重试策略。|

