/**
 * @file    power_allocation_port_input.c
 * @brief   功率分配模块输入接口占位实现
 * @note    所有接口均返回 0，输出参数保持原值。
 */

#include "power_allocation_port_input.h"

uint8_t power_alloc_conn_has_req(uint8_t connector_index)
{
    (void)connector_index;
    return 0U;
}

uint8_t power_alloc_get_conn_req(uint8_t connector_index, CHARGE_POWER_ALLOCATION_PARAM_T *param)
{
    (void)connector_index;
    (void)param;
    return 0U;
}

uint8_t power_allocation_port_input_get_powerunit_module(uint8_t module_index,
                                                         POWER_ALLOCATION_POWERUNIT_DATA_T *data)
{
    (void)module_index;
    (void)data;
    return 0U;
}

uint8_t power_allocation_port_input_get_parallel_contactor(uint8_t *closed)
{
    (void)closed;
    return 0U;
}

uint8_t power_allocation_port_input_ctrl_powerunit_module(uint8_t module_index,
                                                          const POWER_ALLOCATION_POWERUNIT_CTRL_T *ctrl)
{
    (void)module_index;
    (void)ctrl;
    return 0U;
}

/* 动态负载均衡（DLB）限制源输入 */

uint8_t power_allocation_port_input_get_modbus_evse_max_power(uint32_t *max_power_w)
{
    (void)max_power_w;
    return 0U;
}

uint8_t power_allocation_port_input_get_modbus_connector_max_power(uint8_t connector_index,
                                                                   uint32_t *max_power_w)
{
    (void)connector_index;
    (void)max_power_w;
    return 0U;
}

uint8_t power_allocation_port_input_get_ocpp_smart_charging_limit(uint8_t connector_index,
                                                                  POWER_ALLOCATION_DLB_LIMIT_T *limit)
{
    (void)connector_index;
    (void)limit;
    return 0U;
}

uint8_t power_allocation_port_input_get_ocpp_smart_charging_evse_limit(
    POWER_ALLOCATION_DLB_LIMIT_T *limit)
{
    (void)limit;
    return 0U;
}

uint8_t power_allocation_port_input_get_connector_physics_limit(uint8_t connector_index,
                                                                POWER_ALLOCATION_DLB_LIMIT_T *limit)
{
    (void)connector_index;
    (void)limit;
    return 0U;
}

uint8_t power_allocation_port_input_get_user_evse_max_power(uint32_t *max_power_w)
{
    (void)max_power_w;
    return 0U;
}

uint8_t power_allocation_port_input_get_user_connector_max_power(uint8_t connector_index,
                                                                 uint32_t *max_power_w)
{
    (void)connector_index;
    (void)max_power_w;
    return 0U;
}

uint8_t power_allocation_port_input_get_temperature_derating(uint8_t connector_index,
                                                             uint16_t *percent_0d1pct)
{
    (void)connector_index;
    (void)percent_0d1pct;
    return 0U;
}

uint8_t power_allocation_port_input_get_boost(uint8_t connector_index,
                                              POWER_ALLOCATION_BOOST_T *boost)
{
    (void)connector_index;
    (void)boost;
    return 0U;
}
