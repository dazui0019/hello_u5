/**
 * @file    power_allocation_port_app.h
 * @brief   功率分配模块输出接口
 * @note    只读接口：功率分配完成状态与故障状态均由状态机直接推导，
 *          不对外开放任何写入接口，避免外部误改内部状态。
 */

#ifndef POWER_ALLOCATION_PORT_APP_H
#define POWER_ALLOCATION_PORT_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief 功率分配机型段数枚举
 */
typedef enum
{
    POWER_ALLOCATION_SEG_TYPE_1SEG = 0, /**< 单段机 */
    POWER_ALLOCATION_SEG_TYPE_2SEG      /**< 两段机 */
} POWER_ALLOCATION_SEG_TYPE_E;

/**
 * @brief  读取当前机型段数
 * @return POWER_ALLOCATION_SEG_TYPE_E
 * @note   配置枚举有 2 段/4 段，功率分配当前仅实现 1 段/2 段状态机；
 *         配置为 4 段时暂回退为两段机，待四段状态机落地后再按配置分发。
 */
POWER_ALLOCATION_SEG_TYPE_E power_allocation_get_seg_type(void);

/**
 * @brief 功率分配输出接口初始化
 * @note  上电调用一次，复位两段机、四段机状态机；
 *        单段机无内部状态，不需要初始化。
 */
void power_allocation_port_app_init(void);

/**
 * @brief  查询指定连接器功率分配是否完成
 * @param  connector_index 连接器索引，0 代表 1 枪，1 代表 2 枪
 * @retval 1 功率分配完成，该连接器可进入充电
 * @retval 0 未完成、索引非法或机型类型非法
 * @note   根据当前机型的功率分配状态机推导，纯只读、无阻塞；
 *         处于故障状态时返回 0；调用方可周期性轮询。
 */
uint8_t power_allocation_port_app_is_complete(uint8_t connector_index);

/**
 * @brief  查询功率分配是否故障
 * @retval 1 故障，0 正常或机型类型非法
 * @note   接触器、电源模块或模块路由控制确认超时后进入故障状态；
 *         根据当前机型的功率分配状态机推导，纯只读；
 *         故障为锁存状态，两把枪都退出充电后状态机才回收模块并复位。
 */
uint8_t power_allocation_port_app_is_fault(void);

#ifdef __cplusplus
}
#endif

#endif /* POWER_ALLOCATION_PORT_APP_H */
