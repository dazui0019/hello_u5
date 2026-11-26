/**
 * @file    charge_model_types.h
 * @brief   充电模型公共枚举定义
 * @note    各模块共用的枚举统一放在本文件，避免多处重复定义。
 */

#ifndef CHARGE_MODEL_TYPES_H
#define CHARGE_MODEL_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** charge_model 公共控制任务周期 */
#define CHARGE_MODEL_CONTROL_PERIOD_MS 2U

/* ============================================================================
 * 充电状态
 * ==========================================================================*/

/**
 * @brief 枪级公共状态
 * @note  枚举值保持协议定义不变。
 */
typedef enum
{
    CHARGE_STATE_UNKNOWN = 0,          /**< 未知 */
    CHARGE_STATE_IDLE = 1,             /**< 空闲 */
    CHARGE_STATE_PLUGGING = 2,         /**< 已插枪 */
    CHARGE_STATE_START_CHARGE = 3,     /**< 充电启动中 */
    CHARGE_STATE_CHARGING = 4,         /**< 充电中 */
    CHARGE_STATE_CHARGE_FINISHED = 5,  /**< 充电完成/停止 */
    CHARGE_STATE_BOOK = 6,             /**< 预约中 */
    CHARGE_STATE_UNAVAILABLE = 7,      /**< 枪不可用 */
    CHARGE_STATE_WAIT_PLUG = 8,        /**< 鉴权完成，等待插枪 */
    CHARGE_STATE_SUSPEND = 9,          /**< 充电暂停 */
    CHARGE_STATE_FAULT = 0xFF          /**< 故障 */
} CHARGE_STATE_E;

/**
 * @brief 充电启动来源
 * @note  枚举值保持协议定义不变。
 */
typedef enum
{
    CHARGE_START_TYPE_UNKNOWN = 0,  /**< 未知 */
    CHARGE_START_TYPE_LOCAL = 1,    /**< 本地启动 */
    CHARGE_START_TYPE_REMOTE = 2,   /**< 远程启动 */
    CHARGE_START_TYPE_RFID = 3,     /**< RFID 启动 */
    CHARGE_START_TYPE_POS = 4,      /**< POS 启动 */
    CHARGE_START_TYPE_PNC = 5,      /**< 即插即充启动 */
    CHARGE_START_TYPE_WEB = 6,      /**< Web 启动 */
    CHARGE_START_TYPE_PASSWORD = 7  /**< 密码启动 */
} CHARGE_START_TYPE_E;

/**
 * @brief 充电停止原因
 * @note  枚举值保持协议定义不变。
 */
typedef enum
{
    CHARGE_STOP_TYPE_UNKNOWN = 0,                 /**< 未知 */
    CHARGE_STOP_TYPE_LOCAL = 1,                   /**< 本地停止 */
    CHARGE_STOP_TYPE_REMOTE = 2,                  /**< 远程停止 */
    CHARGE_STOP_TYPE_RFID = 3,                    /**< RFID 停止 */
    CHARGE_STOP_TYPE_POS = 4,                     /**< POS 停止 */
    CHARGE_STOP_TYPE_WEB = 5,                     /**< Web 停止 */
    CHARGE_STOP_TYPE_EV_DISCONNECT = 6,           /**< 车端主动停止 */
    CHARGE_STOP_TYPE_REACH_TOTAL_SOC = 7,         /**< 达到设定 SOC */
    CHARGE_STOP_TYPE_PREPAID_BALANCE_USED_UP = 8, /**< 预扣费金额使用完毕 */
    CHARGE_STOP_TYPE_REACH_TOTAL_VOLTAGE = 9,     /**< 达到设定总电压 */
    CHARGE_STOP_TYPE_REACH_CELL_VOLTAGE = 10,     /**< 达到单体电压设定 */
    CHARGE_STOP_TYPE_EV_REASON = 11,              /**< 车端异常停充 */
    CHARGE_STOP_TYPE_DEAUTHORIZED = 12,           /**< 授权取消 */
    CHARGE_STOP_TYPE_SOFT_RESET = 13,             /**< 软复位 */
    CHARGE_STOP_TYPE_HARD_RESET = 14,             /**< 硬复位 */
    CHARGE_STOP_TYPE_POWER_LOSS = 15,             /**< 掉电 */
    CHARGE_STOP_TYPE_REBOOT = 16,                 /**< 重启 */
    CHARGE_STOP_TYPE_VEHICLE_HANDSHAKE_TIMEOUT = 17, /**< 车端握手超时 */
    CHARGE_STOP_TYPE_REACH_TOTAL_ENERGY = 18,     /**< 达到设定电量 */
    CHARGE_STOP_TYPE_FAULT = 0xFF                 /**< 故障停止 */
} CHARGE_STOP_TYPE_E;

/* ============================================================================
 * 枪头类型
 * ==========================================================================*/

/**
 * @brief 枪头类型
 */
typedef enum
{
    CHARGE_CONNECTOR_TYPE_CCS2 = 0, /**< CCS2 */
    CHARGE_CONNECTOR_TYPE_GBT,      /**< GB/T */
    CHARGE_CONNECTOR_TYPE_CCS1,     /**< CCS1 */
    CHARGE_CONNECTOR_TYPE_NACS,     /**< NACS */
    CHARGE_CONNECTOR_TYPE_NONE      /**< 无该枪头：仅允许配置在 2 枪，表示单枪版本 */
} CHARGE_CONNECTOR_TYPE_E;

/* ============================================================================
 * 系统信号
 * ==========================================================================*/

/**
 * @brief 复位信号来源
 */
typedef enum
{
    CHARGE_RESET_SOURCE_NONE = 0,       /**< 无复位信号 */
    CHARGE_RESET_SOURCE_CLOUD,          /**< 云端复位 */
    CHARGE_RESET_SOURCE_LOCAL_SOFTWARE, /**< 本地软件复位 */
    CHARGE_RESET_SOURCE_LOCAL_HARDWARE  /**< 本地硬件复位 */
} CHARGE_RESET_SOURCE_E;

/* ============================================================================
 * 硬件状态
 * ==========================================================================*/

/**
 * @brief 电子锁反馈状态
 */
typedef enum
{
    CHARGE_ELOCK_FEEDBACK_OPEN = 0, /**< 反馈断开/未锁止 */
    CHARGE_ELOCK_FEEDBACK_CLOSED    /**< 反馈闭合/已锁止 */
} CHARGE_ELOCK_FEEDBACK_E;

/**
 * @brief 国标协议下的插枪状态
 */
typedef enum
{
    CHARGE_CONNECTOR_PLUG_STATE_UNPLUGGED = 0, /**< 未插枪 */
    CHARGE_CONNECTOR_PLUG_STATE_PLUGGED,       /**< 已插枪 */
} CHARGE_CONNECTOR_PLUG_STATE_E;

/**
 * @brief 辅助电源通道
 */
typedef enum
{
    CHARGE_AUX_POWER_CHANNEL_12V = 0, /**< 12V辅源通道 */
    CHARGE_AUX_POWER_CHANNEL_24V      /**< 24V辅源通道 */
} CHARGE_AUX_POWER_CHANNEL_E;

#define CHARGE_AUX_POWER_FEEDBACK_CLOSED 0U /**< 辅源反馈闭合 */
#define CHARGE_AUX_POWER_FEEDBACK_OPEN   1U /**< 辅源反馈断开 */
#define CHARGE_AUX_POWER_CONTROL_OPEN    0U /**< 辅源控制断开 */
#define CHARGE_AUX_POWER_CONTROL_CLOSE   1U /**< 辅源控制闭合 */

/**
 * @brief 枪输出继电器状态
 */
typedef enum
{
    CHARGE_RELAY_STATE_OPEN = 0, /**< 断开 */
    CHARGE_RELAY_STATE_CLOSED,   /**< 闭合 */
    CHARGE_RELAY_STATE_FAULT     /**< 继电器故障 */
} CHARGE_RELAY_STATE_E;

/* ============================================================================
 * 车辆电池信息
 * ==========================================================================*/

/**
 * @brief 电池类型
 */
typedef enum
{
	CHARGE_BATTERY_TYPE_UNKNOWN, /**< 未知类型 */
} CHARGE_BATTERY_TYPE_E;

/**
 * @brief 车辆电池信息（对外发布的只读关键数据）
 */
typedef struct
{
    int16_t  soc_percent;              /**< 车辆 SOC，-1 表示未获取 */
    char     vin[18];                  /**< 车辆 VIN，空串表示未获取 */
    uint8_t  mac[6];                   /**< 车辆 MAC，全 0 表示未获取 */
    uint8_t  battery_type;             /**< 协议原始电池类型，0 表示未知 */

    uint16_t max_charge_voltage_0d1v;  /**< 最高允许充电电压，0.1V */
    uint16_t max_charge_current_0d1a;  /**< 最高允许充电电流，0.1A */
    uint16_t target_voltage_0d1v;      /**< 目标充电电压，0.1V */
    uint16_t target_current_0d1a;      /**< 目标充电电流，0.1A */
    uint16_t remaining_time_min;       /**< 剩余充电时间，min */
    uint8_t  max_temperature_c;        /**< 最高温度，℃ */
} CHARGE_VEHICLE_INFO_T;

/**
 * @brief 功率分配参数
 */
typedef struct
{
    uint16_t target_voltage_0d1v;  /**< 目标电压，0.1V */
    uint16_t target_current_0d1a;  /**< 目标电流，0.1A */
} CHARGE_POWER_ALLOCATION_PARAM_T;

/**
 * @brief 充电过程过压、欠压和过流保护计时状态
 */
typedef struct
{
    uint32_t bms_pause_start_tick; /**< BMS 暂停充电开始 tick */
    uint32_t zero_voltage_start_tick; /**< 目标电压为 0 开始 tick */
    uint32_t over_voltage_start_tick; /**< 充电过压开始 tick */
    uint32_t under_voltage_start_tick; /**< 充电欠压开始 tick */
    uint32_t over_current_start_tick; /**< 充电过流开始 tick */
    uint32_t fast_over_current_start_tick; /**< 快速过流开始 tick */
    uint32_t over_current_hold_ms; /**< 当前过流故障保持时间 */
    uint16_t over_current_previous_target_0d1a; /**< 上次过流目标电流 */
} CHARGE_CHARGING_PROTECTION_T;

/**
 * @brief 绝缘检测状态
 */
typedef enum
{
    IMD_CHECK_INVALID = 0, /**< 无效阻值 */
    IMD_CHECK_VALID = 1,   /**< 有效阻值 */
    IMD_CHECK_WARNING = 2, /**< 告警 */
    IMD_CHECK_FAULT = 3,   /**< 故障 */
    IMD_CHECKING = 4       /**< 检测中 */
} IMD_CHECK_STATUS_E;

#ifdef __cplusplus
}
#endif

#endif /* CHARGE_MODEL_TYPES_H */
