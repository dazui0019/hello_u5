/**
 * @file    power_allocation_dlb.c
 * @brief   动态负载均衡（DLB）功率限制计算
 * @note    task 计算逻辑后续人工补全，当前仅维护限制缓存框架。
 */

#include "power_allocation_dlb.h"

#include <stddef.h>
#include <string.h>

#include "charge_global.h"

/* ============================================================================
 * 内部变量
 * ==========================================================================*/

/** 各枪最终 DLB 限制缓存，由 task 计算刷新 */
static POWER_ALLOCATION_DLB_LIMIT_T s_dlb_limit[CHARGE_CONNECTOR_NUM];

/* ============================================================================
 * 接口函数实现
 * ==========================================================================*/

void power_allocation_dlb_init(void)
{
    memset(s_dlb_limit, 0, sizeof(s_dlb_limit));
}

void power_allocation_dlb_task_10ms(void)
{
    /* TODO: 综合以下限制源取最小值，得出各枪功率/电流限制：
     *  1. Modbus EVSE 最大功率（该 EVSE 下各枪共享）
     *     power_allocation_port_input_get_modbus_evse_max_power()
     *  2. Modbus 单枪最大功率
     *     power_allocation_port_input_get_modbus_connector_max_power()
     *  3. OCPP SmartCharging 枪级限制
     *     power_allocation_port_input_get_ocpp_smart_charging_limit()
     *  4. OCPP SmartCharging EVSE 级限制
     *     power_allocation_port_input_get_ocpp_smart_charging_evse_limit()
     *  5. 桩物理限制的每枪最大功率/电流
     *     power_allocation_port_input_get_connector_physics_limit()
     *  6. 用户设定的 EVSE 最大功率
     *     power_allocation_port_input_get_user_evse_max_power()
     *  7. 用户设定的单枪最大功率
     *     power_allocation_port_input_get_user_connector_max_power()
     *  8. 当前功率分配分组模式下该枪 电源模块能力
     *     power_allocation_port_app_get_connector_limit()
     * 再结合：
     *  - 温控降负载系数：功率限制乘法降载
     *    power_allocation_port_input_get_temperature_derating()
     *  - boost 模式及临时提升电流系数：电流限制放大
     *    power_allocation_port_input_get_boost()
     * 计算结果写入 s_dlb_limit，由 power_allocation_dlb_get_limit() 对外提供。
     */
}

uint8_t power_allocation_dlb_get_limit(uint8_t connector_index,
                                       POWER_ALLOCATION_DLB_LIMIT_T *limit)
{
    if ((limit == NULL) || (connector_index >= CHARGE_CONNECTOR_NUM))
    {
        return 0U;
    }

    *limit = s_dlb_limit[connector_index];
    return 1U;
}
