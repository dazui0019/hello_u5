/**
 * @file    smf_port.h
 * @brief   SMF 在非 Zephyr 工程中的配置与编译适配
 */
#ifndef CCU_SMF_PORT_H
#define CCU_SMF_PORT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*
 * 全工程统一配置：0 关闭，1 开启；也可通过全局编译选项覆盖。
 * 不要在某个业务 .c 中单独配置，这些选项会改变 SMF 结构体布局。
 * 默认保持平面状态机，不启用调试钩子。
 */
#ifndef CONFIG_SMF_ANCESTOR_SUPPORT
#define CONFIG_SMF_ANCESTOR_SUPPORT 0
#endif

#ifndef CONFIG_SMF_INITIAL_TRANSITION
#define CONFIG_SMF_INITIAL_TRANSITION 0
#endif

#ifndef CONFIG_SMF_INSTRUMENTATION
#define CONFIG_SMF_INSTRUMENTATION 0
#endif

#if ((CONFIG_SMF_ANCESTOR_SUPPORT != 0) && (CONFIG_SMF_ANCESTOR_SUPPORT != 1)) || \
    ((CONFIG_SMF_INITIAL_TRANSITION != 0) && (CONFIG_SMF_INITIAL_TRANSITION != 1)) || \
    ((CONFIG_SMF_INSTRUMENTATION != 0) && (CONFIG_SMF_INSTRUMENTATION != 1))
#error "SMF options must be 0 or 1"
#endif

#if CONFIG_SMF_INITIAL_TRANSITION && !CONFIG_SMF_ANCESTOR_SUPPORT
#error "CONFIG_SMF_INITIAL_TRANSITION requires CONFIG_SMF_ANCESTOR_SUPPORT"
#endif

/* 上游源码使用 #ifdef；将关闭项变为未定义，避免定义为 0 仍被开启。 */
#if !CONFIG_SMF_ANCESTOR_SUPPORT
#undef CONFIG_SMF_ANCESTOR_SUPPORT
#endif
#if !CONFIG_SMF_INITIAL_TRANSITION
#undef CONFIG_SMF_INITIAL_TRANSITION
#endif
#if !CONFIG_SMF_INSTRUMENTATION
#undef CONFIG_SMF_INSTRUMENTATION
#endif

/*
 * IF_ENABLED(flag, (code))：flag 展开为 1 时去掉 code 的外层括号，
 * flag 为 0 或未定义时不输出代码；支持括号内带逗号的初始化项。
 * 辅助宏使用 SMF_PORT 前缀，避免与其他库冲突。
 */
#ifndef IF_ENABLED
#define SMF_PORT_ENABLED_1 ignored,
#define SMF_PORT_ENABLED_PICK(_ignored, value, ...) value
#define SMF_PORT_ENABLED_TEST(value) SMF_PORT_ENABLED_PICK(value 1, 0, 0)
#define SMF_PORT_ENABLED_EXPAND(flag) SMF_PORT_ENABLED_TEST(SMF_PORT_ENABLED_##flag)
#define SMF_PORT_ENABLED(flag) SMF_PORT_ENABLED_EXPAND(flag)

#define SMF_PORT_EMIT_0(...)
#define SMF_PORT_EMIT_1(...) __VA_ARGS__
#define SMF_PORT_EMIT_EXPAND(value) SMF_PORT_EMIT_##value
#define SMF_PORT_EMIT(value) SMF_PORT_EMIT_EXPAND(value)

#define IF_ENABLED(flag, code) SMF_PORT_EMIT(SMF_PORT_ENABLED(flag)) code
#endif

#endif /* CCU_SMF_PORT_H */
