/**
 * @file    power_allocation_port_app.c
 * @brief   功率分配模块输出接口实现
 * @note    完成状态与故障状态直接读取当前机型的功率分配状态机，
 *          机型由文件内 power_allocation_get_seg_type() 运行时读取。
 *          本文件只提供只读接口，不对外开放任何写入接口。
 */

#include "power_allocation_port_app.h"
#include "cmsis_os2.h"
#include "Libraries/zephyr_smf/smf.h"
#include "app/logs/log.h"

LOG_MODULE_REGISTER(power_alloc_app);

POWER_ALLOCATION_SEG_TYPE_E power_allocation_get_seg_type(void)
{
    // (void)charge_config_electrical_get_seg_mode();

    return POWER_ALLOCATION_SEG_TYPE_2SEG;
}

/**
 * @brief  查询指定连接器功率分配是否完成
 * @param  connector_index 连接器索引，0 代表 1 枪，1 代表 2 枪
 * @retval 1 功率分配完成，该连接器可以进入充电
 * @retval 0 未完成、参数非法或机型类型非法
 * @note   按 get_seg_type() 读取的机型分发到对应段机状态机，纯只读。
 *         功率分配故障时该接口返回 0。
 *         调用方可周期性轮询，接口内部无阻塞。
 */
uint8_t power_allocation_port_app_is_complete(uint8_t connector_index)
{
    return 0;
}

/**
 * @brief  查询功率分配是否处于故障状态
 * @retval 1 故障，0 正常或机型类型非法
 * @note   接触器、电源模块控制确认超时后进入故障状态。
 *         按 get_seg_type() 读取的机型分发到对应段机状态机，纯只读。
 *         单段机没有分配过程，恒返回 0。
 *         故障为锁存状态，两把枪都退出充电后状态机才会自动回收模块并复位。
 */
uint8_t power_allocation_port_app_is_fault(void)
{
    return 0;
}
