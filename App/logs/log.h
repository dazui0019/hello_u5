#ifndef _CCU_LOG_H
#define _CCU_LOG_H

#include "elog.h"

#ifdef __cplusplus
extern "C" {
#endif

void log_init();

/* 业务日志统一使用 Zephyr 风格接口子集，底层输出和运行时过滤沿用 EasyLogger。
 * 每个使用日志的 .c 文件在包含本头文件后注册模块，无需定义 LOG_TAG：
 *   LOG_MODULE_REGISTER(app);                 // 默认 INFO 等级
 *   LOG_MODULE_REGISTER(app, LOG_LEVEL_DBG);  // 或显式指定等级
 * 输出使用 LOG_ERR / LOG_WRN / LOG_INF / LOG_DBG / LOG_VRB。
 * LOG_VRB 为扩展接口，需注册为 LOG_LEVEL_VRB 才会输出。
 */
#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERR  ELOG_LVL_ERROR
#define LOG_LEVEL_WRN  ELOG_LVL_WARN
#define LOG_LEVEL_INF  ELOG_LVL_INFO
#define LOG_LEVEL_DBG  ELOG_LVL_DEBUG
#define LOG_LEVEL_VRB  ELOG_LVL_VERBOSE

/* 可在包含本头文件前覆盖；默认保留 ERROR、WARN、INFO。 */
#ifndef CCU_LOG_DEFAULT_LEVEL
#define CCU_LOG_DEFAULT_LEVEL LOG_LEVEL_INF
#endif

#define CCU_LOG_MODULE_DEFINE(name, level)                    \
    static inline const char *ccu_log_module_tag(void)        \
    {                                                       \
        return #name;                                       \
    }                                                       \
    enum { CCU_LOG_MODULE_LEVEL = (level) }

#define CCU_LOG_MODULE_DEFAULT(name) \
    CCU_LOG_MODULE_DEFINE(name, CCU_LOG_DEFAULT_LEVEL)

#define CCU_LOG_MODULE_SELECT(_name, _level, selected, ...) selected
#define LOG_MODULE_REGISTER(...)                                      \
    CCU_LOG_MODULE_SELECT(__VA_ARGS__, CCU_LOG_MODULE_DEFINE,           \
                          CCU_LOG_MODULE_DEFAULT, unused)(__VA_ARGS__)

/* 多文件模块使用同一名称即可共享 EasyLogger 的 tag 过滤规则。 */
#define LOG_MODULE_DECLARE(...) LOG_MODULE_REGISTER(__VA_ARGS__)

/* 被模块等级关闭时，不求值格式串及其参数；文件、函数、行号仍取业务调用处。 */
#define CCU_LOG_MODULE_WRITE(level, output, ...)              \
    do                                                      \
    {                                                       \
        if ((level) <= CCU_LOG_MODULE_LEVEL)                 \
        {                                                   \
            output(ccu_log_module_tag(), __VA_ARGS__);       \
        }                                                   \
    } while (0)

#define LOG_E(...) CCU_LOG_MODULE_WRITE(LOG_LEVEL_ERR, elog_e, __VA_ARGS__)
#define LOG_W(...) CCU_LOG_MODULE_WRITE(LOG_LEVEL_WRN, elog_w, __VA_ARGS__)
#define LOG_I(...) CCU_LOG_MODULE_WRITE(LOG_LEVEL_INF, elog_i, __VA_ARGS__)
#define LOG_D(...) CCU_LOG_MODULE_WRITE(LOG_LEVEL_DBG, elog_d, __VA_ARGS__)
#define LOG_V(...) CCU_LOG_MODULE_WRITE(LOG_LEVEL_VRB, elog_v, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* _CCU_LOG_H */
