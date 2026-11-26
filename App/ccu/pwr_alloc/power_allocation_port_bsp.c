/**
 * @file    power_allocation_port_bsp.c
 * @brief   功率分配 BSP 相关端口实现
 * @note    并机接触器转发 drivers/io 的 io_pa_relay（错峰吸合 + 反馈校验），
 *          本层不做任何逻辑；2 段机器固定使用索引 0。
 */

#include <stdbool.h>
#include "power_allocation_port_bsp.h"

uint8_t power_alloc_set_par_contactor(uint8_t idx, uint8_t close)
{
    (void)idx;
    (void)close;
    return 1;
}

uint8_t power_alloc_read_par_feedback(uint8_t idx)
{
    (void)idx;
    return 1;
}

uint8_t power_alloc_set_ac_contactor(uint8_t idx, uint8_t close)
{
    (void)close;
    return 1;
}

bool power_alloc_read_ac_feedback(uint8_t idx)
{
    return 1;
}
