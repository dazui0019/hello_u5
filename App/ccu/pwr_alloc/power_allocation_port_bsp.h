/**
 * @file    power_allocation_port_bsp.h
 * @brief   功率分配 BSP 相关端口
 * @note    只包含与板级硬件直接相关的输出动作，实现全部转发 ports/board 的板级原语；
 *          接触器状态回读、电源模块控制等仍留在 power_allocation_port_input。
 */

#ifndef POWER_ALLOCATION_PORT_BSP_H
#define POWER_ALLOCATION_PORT_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief  控制并机接触器（中间继电器）
 * @param  idx 接触器编号
 * @param  close 1 吸合，0 断开
 * @retval 1 控制成功
 * @retval 0 控制失败
 * @note   只下发命令，不等待动作完成；
 *         动作结果由 power_allocation_port_input_get_parallel_contactor() 回读确认，
 *         状态机等待确认期间有超时保护，超时进入故障状态。
 */
uint8_t power_alloc_set_par_contactor(uint8_t idx, uint8_t close);

/**
 * @brief 读并机接触器反馈
 * @param  idx 接触器编号
 * @retval 1 闭合
 * @retval 0 断开
 */
uint8_t power_alloc_read_par_feedback(uint8_t idx);


uint8_t power_alloc_set_ac_contactor(uint8_t idx, uint8_t close);

bool power_alloc_read_ac_feedback(uint8_t idx);

#ifdef __cplusplus
}
#endif

#endif /* POWER_ALLOCATION_PORT_BSP_H */
