/**
 * @file    power_allocation_port_input.h
 * @brief   功率分配模块输入接口
 * @note    功率分配模块通过该接口读取充电模型提供的分配条件与参数，
 *          以及读写 电源模块、并机接触器等硬件资源。
 *          并机接触器的吸合/断开动作已拆到 power_allocation_port_bsp；
 *          段数类型、模块组分配等静态配置直接读 app/storage/config 的 charge_config_electrical；
 *          电源模块控制与回读、接触器反馈等仍由外部程序在本层提供；
 *          DLB 限制源由 Modbus/OCPP/设置等外部程序提供。
 */

#ifndef POWER_ALLOCATION_PORT_INPUT_H
#define POWER_ALLOCATION_PORT_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "app/ccu/charge_model/charge_model_types.h"

/**
 * @brief 电源模块回读数据
 * @note  数据由硬件采集，功率分配状态机用于判断停机、零流、输出建立等条件。
 */
typedef struct
{
    uint16_t output_voltage_0d1v;  /**< 模块输出电压，0.1V */
    uint16_t output_current_0d1a;  /**< 模块输出电流，0.1A */
    uint8_t  running;              /**< 1 模块运行中，0 未运行 */
    uint8_t  fault;                /**< 1 模块故障，0 正常 */
} POWER_ALLOCATION_POWERUNIT_DATA_T;

/**
 * @brief 电源模块控制参数
 */
typedef struct
{
    uint8_t  power_on;          /**< 1 开机，0 关机 */
    uint16_t max_voltage_0d1v;  /**< 车辆最大允许电压，0.1V */
    uint16_t voltage_0d1v;      /**< 输出电压设定，0.1V */
    uint16_t current_0d1a;      /**< 输出电流设定，0.1A */
} POWER_ALLOCATION_POWERUNIT_CTRL_T;

/**
 * @brief DLB 限制源数值
 * @note  valid 为 0 表示该限制源无效/未下发，DLB 计算时忽略；
 *        无限制的项：功率填 0xFFFFFFFF，电流填 0xFFFF（表示不参与取最小值）。
 */
typedef struct
{
    uint8_t  valid;            /**< 0 该限制源无效，1 有效 */
    uint32_t max_power_w;      /**< 最大功率限制，1W，无限制填 0xFFFFFFFF */
    uint16_t max_current_0d1a; /**< 最大电流限制，0.1A，无限制填 0xFFFF */
} POWER_ALLOCATION_DLB_LIMIT_T;

/**
 * @brief boost 模式状态
 */
typedef struct
{
    uint8_t  enabled;           /**< 1 处于 boost 模式，0 否 */
    uint16_t current_coeff_0d1pct; /**< 临时提升电流系数，0.1% 单位，1200 表示 120.0% */
} POWER_ALLOCATION_BOOST_T;

/**
 * @brief  查询指定连接器当前是否需要开始功率分配
 * @param  connector_index 连接器索引，0 代表 1 枪，1 代表 2 枪
 * @retval 1 允许开始功率分配，0 不允许
 * @note   直接转发充电模型输出接口，条件由充电状态机维护。
 */
uint8_t power_alloc_conn_has_req(uint8_t connector_index);

/**
 * @brief  获取指定连接器的功率分配参数
 * @param  connector_index 连接器索引
 * @param  param           参数输出：目标电压、目标电流
 * @retval 1 获取成功，0 参数非法
 * @note   param 为 NULL 时返回 0；参数由充电状态机统一维护。
 */
uint8_t power_alloc_get_conn_req(uint8_t connector_index,
                                              CHARGE_POWER_ALLOCATION_PARAM_T *param);

/**
 * @brief  回读指定 电源模块的运行数据
 * @param  module_index 电源模块全局索引
 * @param  data         回读数据输出
 * @retval 1 读取成功
 * @retval 0 参数非法或读取失败
 * @note   data 为 NULL 时返回 0；失败时 data 已被清零，
 *         调用方应先判断返回值再使用数据。
 */
uint8_t power_allocation_port_input_get_powerunit_module(uint8_t module_index,
                                                    POWER_ALLOCATION_POWERUNIT_DATA_T *data);

/**
 * @brief  读取并机接触器（中间继电器）状态
 * @param  closed 状态输出，1 已吸合，0 已断开
 * @retval 1 读取成功
 * @retval 0 参数非法或读取失败
 * @note   closed 为 NULL 时返回 0；失败时输出被置 0；
 *         状态机用该反馈确认动作完成，等待超时进入故障状态。
 */
uint8_t power_allocation_port_input_get_parallel_contactor(uint8_t *closed);

/**
 * @brief  控制 电源模块
 * @param  module_index 电源模块全局索引
 * @param  ctrl         控制参数：开关机、车辆最大电压、输出电压、输出电流
 * @retval 1 控制成功
 * @retval 0 参数非法或控制失败
 * @note   ctrl 为 NULL 时返回 0；
 *         开关机、限压、稳压、限流一次性下发，避免分步下发造成中间状态；
 *         只下发命令，输出是否建立由 get_powerunit_module() 回读确认，
 *         确认等待有超时保护，超时进入故障状态。
 */
uint8_t power_allocation_port_input_ctrl_powerunit_module(uint8_t module_index,
                                                     const POWER_ALLOCATION_POWERUNIT_CTRL_T *ctrl);

/* ============================================================================
 * 动态负载均衡（DLB）限制源输入
 * ==========================================================================*/

/**
 * @brief  读取 Modbus 下发的 EVSE 最大功率
 * @param  max_power_w 最大功率输出，1W
 * @retval 1 获取成功，0 未下发或参数非法
 * @note   EVSE 级限制，该 EVSE 下所有枪共享该功率上限。
 */
uint8_t power_allocation_port_input_get_modbus_evse_max_power(uint32_t *max_power_w);

/**
 * @brief  读取 Modbus 下发的单枪最大功率
 * @param  connector_index 连接器索引，0 代表 1 枪，1 代表 2 枪
 * @param  max_power_w 最大功率输出，1W
 * @retval 1 获取成功，0 未下发或参数非法
 */
uint8_t power_allocation_port_input_get_modbus_connector_max_power(uint8_t connector_index,
                                                                   uint32_t *max_power_w);

/**
 * @brief  读取 OCPP SmartCharging 对该枪的限制
 * @param  connector_index 连接器索引
 * @param  limit           限制输出：功率/电流限制
 * @retval 1 获取成功，0 未下发或参数非法
 * @note   OCPP 1.6J ChargingProfile 下发的限制，可能只有功率或只有电流，
 *         无限制的项填 0xFFFF。
 */
uint8_t power_allocation_port_input_get_ocpp_smart_charging_limit(uint8_t connector_index,
                                                                  POWER_ALLOCATION_DLB_LIMIT_T *limit);

/**
 * @brief  读取 OCPP SmartCharging 对 EVSE 的限制
 * @param  limit 限制输出：EVSE 级功率/电流限制
 * @retval 1 获取成功，0 未下发或参数非法
 * @note   OCPP 1.6J ChargingProfile 中 EVSE 级 Profile 下发的限制，
 *         对该 EVSE 下所有枪生效；可能只有功率或只有电流，
 *         无限制的项填 0xFFFF。
 */
uint8_t power_allocation_port_input_get_ocpp_smart_charging_evse_limit(
    POWER_ALLOCATION_DLB_LIMIT_T *limit);

/**
 * @brief  读取桩物理限制的每枪最大功率/电流
 * @param  connector_index 连接器索引
 * @param  limit           限制输出：物理最大功率、物理最大电流
 * @retval 1 获取成功，0 参数非法
 * @note   由桩硬件规格决定的硬上限，与分配模式无关。
 */
uint8_t power_allocation_port_input_get_connector_physics_limit(uint8_t connector_index,
                                                                POWER_ALLOCATION_DLB_LIMIT_T *limit);

/**
 * @brief  读取用户设定的 EVSE 最大功率
 * @param  max_power_w 最大功率输出，1W
 * @retval 1 获取成功，0 未设定或参数非法
 * @note   EVSE 级设置，该 EVSE 下所有枪共享。
 */
uint8_t power_allocation_port_input_get_user_evse_max_power(uint32_t *max_power_w);

/**
 * @brief  读取用户设定的单枪最大功率
 * @param  connector_index 连接器索引
 * @param  max_power_w 最大功率输出，1W
 * @retval 1 获取成功，0 未设定或参数非法
 */
uint8_t power_allocation_port_input_get_user_connector_max_power(uint8_t connector_index,
                                                                 uint32_t *max_power_w);

/**
 * @brief  读取当前温控降负载系数
 * @param  connector_index 连接器索引
 * @param  percent_0d1pct     系数输出，0.1% 单位，1000 表示 100.0%（不降载）
 * @retval 1 获取成功，0 参数非法
 * @note   由温控程序按枪内温度计算，DLB 用该系数对功率限制做乘法降载。
 */
uint8_t power_allocation_port_input_get_temperature_derating(uint8_t connector_index,
                                                             uint16_t *percent_0d1pct);

/**
 * @brief  读取该枪 boost 模式状态与临时提升电流系数
 * @param  connector_index 连接器索引
 * @param  boost           状态输出：是否 boost、提升电流系数
 * @retval 1 获取成功
 * @retval 0 参数非法
 * @note   boost 字段为 NULL 时返回 0；
 *         boost 关闭时 current_coeff_0d1pct 应填 1000（100.0%，即不放大）。
 */
uint8_t power_allocation_port_input_get_boost(uint8_t connector_index,
                                              POWER_ALLOCATION_BOOST_T *boost);

#ifdef __cplusplus
}
#endif

#endif /* POWER_ALLOCATION_PORT_INPUT_H */
