#ifndef _POWER_ALLOCATION_TYPES_H
#define _POWER_ALLOCATION_TYPES_H

#include <stdbool.h>

#include "charge_global.h"
#include "app/ccu/charge_model/charge_model_types.h"
#include "Libraries/zephyr_smf/smf.h"

struct power_alloc_pwr_req
{
    bool req_valid;  /* 本拍是否成功读到需求功率参数 */
    CHARGE_POWER_ALLOCATION_PARAM_T pwr_req;
};

struct ContactorOperation
{
    uint32_t target_mask;
    uint32_t feedback_mask;
    uint32_t start_tick;
};

struct power_alloc_pdu_power
{
    uint32_t target_mask;
    uint32_t feedback_mask;
    uint32_t start_tick;
};

enum power_alloc_mode
{
    ALLOC_MODE_NONE = 0,  /* 尚未选定供电方案 */
    ALLOC_MODE_A_ONLY,    /* PAR 断开，仅 A 组给 C1 供电 */
    ALLOC_MODE_B_ONLY,    /* PAR 断开，仅 B 组给 C2 供电 */
    ALLOC_MODE_PAR,       /* PAR 闭合，A、B 两组给目标枪供电 */
    ALLOC_MODE_SPLIT,     /* PAR 断开，A 组给 C1、B 组给 C2 供电 */
};

struct power_alloc_target
{
    uint8_t conn_mask; /* 本轮服务的枪掩码 */
    bool par_closed;  /* 本轮 PAR 目标，false 为断开 */
    uint8_t pdu_mask;  /* 本轮计划上电的 PDU 组掩码 */
    uint8_t keep_pdu_mask;  /* 模块释放流程中需要保留的模块 */
    uint8_t release_pdu_mask;   /* 模块释放流程中需要释放的模块 */
};

/* 状态机实例：框架上下文 + 业务数据 */
struct power_alloc_machine_ctx
{
    struct smf_ctx ctx; /* 必须放在第一个成员 */
    struct power_alloc_pwr_req conn_pwr_reqs[CHARGE_CONNECTOR_NUM];
    uint8_t req_conn_mask; /* 本拍申请资源的枪，bit0 对应 C1，bit1 对应 C2 */
    enum power_alloc_mode run_mode;
    struct power_alloc_target target;
    uint32_t state_start_tick;
    bool par_close_failed; /* 本轮分配尝试吸合 PAR 失败 */
};

#endif /* _POWER_ALLOCATION_TYPES_H */
