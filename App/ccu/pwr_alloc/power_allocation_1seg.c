/**
 * @file    power_allocation_1seg.c
 * @brief   单段功率分配实现
 */

#include "power_allocation_1seg.h"

#include "power_allocation_port_input.h"

void power_allocation_1seg_task_10ms(void)
{
    /* TODO: 单段功率分配逻辑 */
}

uint8_t power_allocation_1seg_is_complete(uint8_t connector_index)
{
    /* 单段机模块与枪固定连接，无分配过程，直接跟随启动条件 */
    return power_alloc_conn_has_req(connector_index);
}

uint8_t power_allocation_1seg_is_fault(void)
{
    /* 单段机无分配过程，无故障状态 */
    return 0U;
}
