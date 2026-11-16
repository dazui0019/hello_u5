/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */
 
#include "elog.h"
#include "log.h"
#include "printf.h"

#include "main.h"
#include "usart.h"
#include "cmsis_os2.h"

static osMutexId_t outputMutex;

/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;

    /* 同步 LOG_x 和异步 logger 任务共用 EasyLogger 的格式化缓冲区。 */
    outputMutex = osMutexNew(NULL);
    if (outputMutex == NULL) Error_Handler();
    
    return result;
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void) {
    if (outputMutex != NULL) {
        osMutexDelete(outputMutex);
        outputMutex = NULL;
    }
}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
    
    /* add your code here */
    HAL_UART_Transmit(&huart4, (uint8_t*)log, size, 100);
    // todo: 串口可以显示颜色, 如果保存到SD卡则不要颜色
}

/**
 * output lock
 */
void elog_port_output_lock(void) {
    /* 初始化提示在启动调度器之前输出，此时没有任务并发。 */
    osKernelState_t state = osKernelGetState();
    if (state == osKernelRunning || state == osKernelLocked) {
        /* 调度器锁定时不能等待别的任务释放互斥锁。 */
        uint32_t timeout = state == osKernelLocked ? 0 : osWaitForever;
        if (osMutexAcquire(outputMutex, timeout) != osOK) Error_Handler();
    }
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    osKernelState_t state = osKernelGetState();
    if (state == osKernelRunning || state == osKernelLocked) {
        if (osMutexRelease(outputMutex) != osOK) Error_Handler();
    }
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    /* EasyLogger 在 outputMutex 内调用；字符串转换仍留在输出阶段。 */
    static char timeText[16];
    uint32_t timestamp_ms = log_get_output_timestamp_ms();
    snprintf(timeText, sizeof(timeText), "%lu.%03lu",
             (unsigned long)(timestamp_ms / 1000U),
             (unsigned long)(timestamp_ms % 1000U));
    return timeText;
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {
    
    /* add your code here */
    return "process_name";
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {
    
    /* add your code here */
    return "task_name";
}
