/**
 * @file    smf_cfg.h
 * @brief   SMF 功能配置
 */
#ifndef CCU_SMF_CFG_H
#define CCU_SMF_CFG_H

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

#endif /* CCU_SMF_CFG_H */
