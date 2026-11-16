#include "log.h"
#include "cbprintf.h"
#include <stdarg.h>
#include "cmsis_os2.h"
#include "main.h"
LOG_MODULE_REGISTER(ccu_log);

static osThreadId_t loggerTaskHandle;
static osMessageQueueId_t logQueue;
static osMutexId_t submitMutex;
static uint32_t loggerTimestampMs;  // 当前被打印消息的时间戳

typedef struct {
    uint8_t level;
    uint32_t timestamp_ms;
    const char *tag;
    const char *file;
    const char *function;
    long line;
    _Alignas(CBPRINTF_PACKAGE_ALIGNMENT) unsigned char package[LOG_ASYNC_PACKAGE_SIZE];
} LogMessage;

/* 生产者共享此缓冲区，由 submitMutex 保护；队列复制整条消息。 */
static LogMessage pendingMessage;
static char outputText[ELOG_LINE_BUF_SIZE];
const static osThreadAttr_t loggerTask_attributes = {
  .name = "logTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 512 * 4
};

int log_async_submit(uint8_t level, const char *tag, const char *file,
                     const char *function, long line, const char *format, ...)
{
    /* 入口立即采样，打包和排队耗时不计入日志发生时间。 */
    uint32_t timestamp_ms = HAL_GetTick();
    if (__get_IPSR() != 0 || loggerTaskHandle == NULL) return -EPERM;
    osKernelState_t state = osKernelGetState();
    if (state != osKernelRunning && state != osKernelLocked) return -EPERM;
    if (format == NULL || tag == NULL || level > ELOG_LVL_VERBOSE) return -EINVAL;
    if (osMutexAcquire(submitMutex, 0) != osOK) return -EBUSY;

    va_list args;
    va_start(args, format);
    int length = cbvprintf_package(pendingMessage.package,
                                  sizeof(pendingMessage.package), 0, format, args);
    va_end(args);
    if (length >= 0) {
        pendingMessage.level = level;
        pendingMessage.timestamp_ms = timestamp_ms;
        pendingMessage.tag = tag;
        pendingMessage.file = file;
        pendingMessage.function = function;
        pendingMessage.line = line;
        /* 复制的是消息本体，源缓冲区解锁后可以立即复用。 */
        if (osMessageQueuePut(logQueue, &pendingMessage, 0, 0) != osOK) {
            length = -EAGAIN;
        }
    }
    osMutexRelease(submitMutex);
    return length;
}

uint32_t log_get_output_timestamp_ms(void)
{
    osKernelState_t state = osKernelGetState();
    if (__get_IPSR() == 0 && loggerTaskHandle != NULL &&
        (state == osKernelRunning || state == osKernelLocked) &&
        osThreadGetId() == loggerTaskHandle) {
        return loggerTimestampMs;
    }
    /* 其他任务同步打印时，独立采集当前时间，不使用异步消息的时间。 */
    return HAL_GetTick();
}

static int LogOutputChar(int character, void *context)
{
    size_t *length = context;
    if (*length + 1 >= sizeof(outputText)) return -ENOSPC;
    outputText[(*length)++] = (char)character;
    outputText[*length] = '\0';
    return character;
}

void StartLoggerTask(void *argument)
{
    (void)argument;
    static LogMessage message;
    for (;;) {
        if (osMessageQueueGet(logQueue, &message, NULL, osWaitForever) != osOK) continue;
        loggerTimestampMs = message.timestamp_ms;
        size_t length = 0;
        outputText[0] = '\0';
        int result = cbpprintf(LogOutputChar, &length, message.package);
        if (result >= 0) {
            /* cbpprintf 生成正文；EasyLogger 补齐等级、模块、来源并输出。 */
            elog_output(message.level, message.tag, message.file,
                        message.function, message.line, "%s", outputText);
        } else {
            elog_output(ELOG_LVL_ERROR, "logger", NULL, NULL, 0,
                        "async format failed: %d", result);
        }
    }
}

void log_init()
{
    if (loggerTaskHandle != NULL) return;
    submitMutex = osMutexNew(NULL);
    logQueue = osMessageQueueNew(LOG_ASYNC_QUEUE_LENGTH, sizeof(LogMessage), NULL);
    if (submitMutex == NULL || logQueue == NULL) Error_Handler();
    elog_init();
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_start();

    loggerTaskHandle = osThreadNew(StartLoggerTask, NULL, &loggerTask_attributes);
    if (loggerTaskHandle == NULL) Error_Handler();
}
