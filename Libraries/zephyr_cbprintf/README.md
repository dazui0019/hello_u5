# Zephyr cbprintf 独立适配

基于本目录引入的 Zephyr v4.5.0 三个源文件，补充同版本的
`cbprintf_enums.h` 和 Apache-2.0 LICENSE。适配目标为 STM32U585 / Cortex-M33、
Arm GNU Toolchain、GNU C11；另外支持 Windows x64 GCC 执行主机测试。

## 文件与责任

上游文件留在根目录，平台适配集中在 `port/`：

```text
zephyr_cbprintf/
├─ cbprintf.h
├─ cbprintf_internal.h
├─ cbprintf_enums.h
├─ cbprintf_packaged.c
├─ LICENSE
├─ README.md
└─ port/
   ├─ cbprintf_config.h
   ├─ cbprintf_port.h
   ├─ cbprintf_port.c
   ├─ cbprintf_port_macros.h
   └─ compat/zephyr/
```

- `cbprintf_packaged.c`：上游运行时打包、转换、解包代码。
- `cbprintf.h` / `cbprintf_internal.h`：上游 API 和静态打包宏。
- `port/cbprintf_config.h`：替代 Kconfig，关闭的 `CONFIG_*` 保持未定义。
- `port/compat/zephyr/`：提供原始 `#include <zephyr/...>` 路径的兼容头。
- `port/cbprintf_port.h` / `port/cbprintf_port_macros.h`：编译器属性、断言及必要工具宏。
- `port/cbprintf_port.c`：只读区判断、诊断钩子、现有 `vfctprintf()` 格式化器桥接。
- `tests/test_cbprintf.c`：主机和 MCU 共用的自测。
- `tests/host_packaged.c`：仅用于 Windows 主机测试的 ABI 选择包装。

三个原始文件保持引入时的内容，不修改其算法、头文件路径或条件编译。
兼容头将 `linker_is_in_rodata`、断言和组件诊断连接到适配层。

组件已加入项目 CMake，但尚未接入 `LOG_*`，不创建 FreeRTOS 队列或任务。
现有日志仍按原路径执行同步格式化与输出。

## 运行时打包示例

```c
#include "cbprintf.h"

/* 实际异步系统可由固定缓冲池提供，避免在业务任务栈上放大数组。 */
static _Alignas(CBPRINTF_PACKAGE_ALIGNMENT) unsigned char message[128];
static char output_text[128];
static size_t output_length;

static int output_char(int character, void *context)
{
    (void)context;
    if (output_length + 1 >= sizeof(output_text)) {
        return -ENOSPC;
    }
    output_text[output_length++] = (char)character;
    output_text[output_length] = '\0';
    return character;
}

void example(void)
{
    char state[] = "ready";
    int length = cbprintf_package(message, sizeof(message), 0,
                                 "voltage=%d state=%s", 3300, state);
    if (length < 0) {
        return; /* 缓冲区不足等错误，不能消费这个包。 */
    }
    state[0] = 'X';
    /* 稍后在消费者上下文中调用；state 的原始内容已经复制进 message。 */
    output_length = 0;
    cbpprintf(output_char, NULL, message);
    /* output_text 为 "voltage=3300 state=ready"。 */
}
```

示例为单次调用，静态缓冲区不可被多个生产者并发复用。实际输出可参照现有 `elog_port_output()`
使用有长度的缓冲区批量发送；不建议每个字符都阻塞调用 HAL。

`cbprintf_package(NULL, 0, flags, fmt, ...)` 可查询所需长度。实际包及搬移后的地址应
保持 `CBPRINTF_PACKAGE_ALIGNMENT` 对齐。格式串必须使用寿命足够长的常量；运行时
打包默认将格式串视为只读。RAM 中的 `%s` 参数会复制，`.rodata` 中的字符串保留指针。
传入的字符串必须在打包期间有效且不被并发修改。`%p` 请传 `void *`。
解包时会修正包内字符串指针，同一个包不要被多个消费者同时处理。

## 静态打包

支持 `CBPRINTF_STATIC_PACKAGE`，工具宏上限为 **16 项（含格式串）**。静态参数不要
使用 `i++` 等有副作用的表达式；可先计算到局部变量再打包。
仅数值参数可直接静态打包。含可变字符串时，需要
`CBPRINTF_PACKAGE_ADD_RW_STR_POS`，随后通过 `cbprintf_package_copy()` 和
`CBPRINTF_PACKAGE_CONVERT_RW_STR` 转成自包含包，才能让源字符串失效。
这一步应在生产者提交消息之前完成。

保留原始源码后，也保留了上游的调用约束：格式串和 `%s` 参数必须非 NULL。
静态打包必须先用 NULL 目标查询长度，确认容量足够后再实际打包；不要依靠
其小缓冲区失败路径保护内存。调用 `cbprintf_package_copy()` 前也应先查询转换后的
长度并保证目标容量足够，避免依赖上游简单复制分支的错误返回。

不支持 C++、tagged arguments、`long double` 和 libc 替换。格式化能力仍由
`Libraries/printf/printf.c` 决定；不要直接引入其他版本的包格式或格式化器。

## 平台接口

STM32 的只读区来自 `STM32U585xx_FLASH.ld` 中 `.rodata` 的
`__cbprintf_rodata_start/end`。换链接脚本（包括 RAM 执行）时需要同步提供这些符号，
或替换 `cbprintf_port_is_rodata()`。主机测试保守地复制所有字符串参数。

MCU 上可覆盖三个弱钩子：`cbprintf_port_is_rodata()`、
`cbprintf_port_assert_fail()`、`cbprintf_port_warn()`。断言默认 trap，警告默认不输出；
钩子不得调用依赖此打包器的日志接口，否则会递归。

此前直接加在上游文件中的 NULL 处理、静态边界检查和复制错误传播修正已撤回。
Windows x64 测试通过独立包装选择指针式 `va_list` 分支，原始源码不变。

## 验证

工程编译（CMake 已包含根目录、`port/` 和 `port/compat/`）：

```powershell
cmake --preset Debug
cmake --build --preset Debug
```

以下历史自测命令需要相应的 `tests/` 源码及 CMake 自测开关；当前目录未包含它们。

项目根目录，主机 GCC：

```powershell
gcc -std=gnu11 -O0 -g -Wall -Wextra -Werror `
  -DPRINTF_DISABLE_SUPPORT_PUTCHAR -DCBPRINTF_TEST_MAIN `
  -I Libraries/zephyr_cbprintf -I Libraries/zephyr_cbprintf/port `
  -I Libraries/zephyr_cbprintf/port/compat -I Libraries/printf `
  Libraries/zephyr_cbprintf/tests/host_packaged.c `
  Libraries/zephyr_cbprintf/port/cbprintf_port.c Libraries/printf/printf.c `
  Libraries/zephyr_cbprintf/tests/test_cbprintf.c -o build/Debug/cbprintf_test.exe
./build/Debug/cbprintf_test.exe
```

MCU 自测构建（默认关闭，不加入日常 Debug 固件）：

```powershell
cmake --preset Debug -B build/cbprintf-selftest -DCBPRINTF_BUILD_SELFTEST=ON
cmake --build build/cbprintf-selftest
```

下载 `build/cbprintf-selftest/hello_u5.elf` 后，在 GDB 停于 `main`、内核启动前执行
`p cbprintf_port_selftest()`：返回 0 表示通过，其他正值是失败的测试源码行号。
自测不会自动在业务任务里运行。测试完成后下载普通 Debug 固件即可。

Debug `.su` 报告中 `cbvprintf_package` 的自身栈帧为 152 字节，
`cbprintf_package` 为 44 字节；它们不代表包含所有被调函数的总峰值。
真正的异步日志还需要缓冲池、队列和消费者任务；本次只完成打包层适配。
