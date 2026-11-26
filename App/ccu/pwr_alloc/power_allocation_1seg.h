/**
 * @file    power_allocation_1seg.h
 * @brief   单段功率分配
 */

#ifndef POWER_ALLOCATION_1SEG_H
#define POWER_ALLOCATION_1SEG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief 单段功率分配任务
 */
void power_allocation_1seg_task_10ms(void);

/**
 * @brief  查询指定连接器功率分配是否完成
 * @param  connector_index 连接器索引
 * @retval 1 已完成，0 未完成
 * @note   单段机模块固定连接，无分配过程，直接跟随启动条件。
 */
uint8_t power_allocation_1seg_is_complete(uint8_t connector_index);

/**
 * @brief  查询单段机是否处于故障状态
 * @retval 1 故障，0 正常
 */
uint8_t power_allocation_1seg_is_fault(void);

#ifdef __cplusplus
}
#endif

#endif /* POWER_ALLOCATION_1SEG_H */
