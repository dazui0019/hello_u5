#ifndef APP_LOG_CFG_H
#define APP_LOG_CFG_H

/* LOG_E/W/I/D/V 的默认输出方式：0 同步，1 打包入队后异步输出。
 * 显式 LOG_ASYNC_x 接口始终异步，不受这个开关影响。
 */
#ifndef LOG_USE_ASYNC
#define LOG_USE_ASYNC 1
#endif

/* 异步日志队列可容纳的消息条数；可在此修改或通过全局编译定义覆盖。 */
#ifndef LOG_ASYNC_QUEUE_LENGTH
#define LOG_ASYNC_QUEUE_LENGTH 4U
#endif

/* 每条异步日志的参数包容量，单位为字节，包含参数、字符串副本和对齐填充。 */
#ifndef LOG_ASYNC_PACKAGE_SIZE
#define LOG_ASYNC_PACKAGE_SIZE 128U
#endif

#if LOG_ASYNC_QUEUE_LENGTH < 1
#error "LOG_ASYNC_QUEUE_LENGTH must be at least 1"
#endif

#if (LOG_USE_ASYNC != 0) && (LOG_USE_ASYNC != 1)
#error "LOG_USE_ASYNC must be 0 or 1"
#endif

#if LOG_ASYNC_PACKAGE_SIZE < 1
#error "LOG_ASYNC_PACKAGE_SIZE must be at least 1"
#endif

#endif
