/**
 * @file    power_allocation_dlb.h
 * @brief   动态负载均衡（DLB）功率限制计算
 * @note    汇总各限制源（Modbus 整桩/单枪、OCPP SmartCharging、桩物理限制、
 *          用户设定、当前分配分组模式下 电源模块能力）取最小值，
 *          再结合温控降负载系数与 boost 模式临时提升电流系数，
 *          得出每枪最终功率/电流限制，经 output port 提供给外部程序。
 */

#ifndef POWER_ALLOCATION_DLB_H
#define POWER_ALLOCATION_DLB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "power_allocation_port_input.h"

/**
 * @brief DLB 初始化
 * @note  上电调用一次，清空各枪最终限制缓存。
 */
void power_allocation_dlb_init(void);

/**
 * @brief DLB 计算任务
 * @note  周期调用；内部综合各限制源取最小值并应用温控/boost 系数，
 *        计算逻辑后续人工补全。
 */
void power_allocation_dlb_task_10ms(void);

/**
 * @brief  读取指定枪最终功率/电流限制
 * @param  connector_index 连接器索引，0 代表 1 枪，1 代表 2 枪
 * @param  limit           限制输出：最大功率、最大电流
 * @retval 1 读取成功
 * @retval 0 参数非法
 * @note   结果为 power_allocation_dlb_task_10ms() 最近一次计算的缓存值。
 */
uint8_t power_allocation_dlb_get_limit(uint8_t connector_index,
                                       POWER_ALLOCATION_DLB_LIMIT_T *limit);

#ifdef __cplusplus
}
#endif

#endif /* POWER_ALLOCATION_DLB_H */
