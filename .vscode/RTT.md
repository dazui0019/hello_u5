# OpenOCD + Cortex-Debug RTT

在“运行和调试”选择 **STM32: Debug + RTT (OpenOCD)**，按 F5。
会自动编译、下载并停在 `main`；继续运行后，原生 **STM32 RTT** 终端显示通道 0 日志。
只需断点调试时可以选择 **STM32: Debug (OpenOCD)**。

- `openocd.cfg`：CMSIS-DAP 探针、SWD 接口和 STM32U5 芯片配置；首次 RTT
  检查允许缓冲区尚未初始化，由插件继续重试，避免中断 Live Watch 初始化。
- `launch.json`：Cortex-Debug 原生的 OpenOCD、FreeRTOS、Live Watch 和 RTT 配置。
- `tasks.json`：编译、Rebuild 和 OpenOCD 下载任务；状态栏下载按钮使用同一任务。
- `settings.json` 的 `cortex-debug.openocdPath`：OpenOCD 可执行文件的统一路径。
  更换安装位置时改此处即可；VS Code 已加载 PATH 后，也可以改成 `openocd`。

不需要项目专用的 Node.js/Python 桥接脚本。当前使用 xPack OpenOCD 0.12.0-7、
Cortex-Debug 1.12.1；RTT 初始化重试由 Cortex-Debug 原生处理。

结束调试后再使用下载按钮，避免两个 OpenOCD 进程同时占用探针。
RTT 与串口分别输出时可同时使用；不要再由 MCU 的 `SEGGER_RTT_ReadUpBuffer()`
消费电脑端正在读取的同一个 Up 缓冲区。
