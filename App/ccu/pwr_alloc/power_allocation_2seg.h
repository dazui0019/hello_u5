/**
 * @file    power_allocation_2seg.h
 * @brief   两段功率分配状态机
 * @note    命名约定：模块组用 A/B（并机后为 AB），枪即连接器，用
 *          CONNECTOR1 / CONNECTOR2 表示，避免"枪 A / 模块 A"的歧义。
 */

#ifndef POWER_ALLOCATION_2SEG_H
#define POWER_ALLOCATION_2SEG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "power_allocation_types.h"

/** 两段机, 枪连接器索引 */
#define POWER_ALLOC_2SEG_CONN1_INDEX 0U
#define POWER_ALLOC_2SEG_CONN2_INDEX 1U

/**
 * @brief 两段机任务调用周期，ms
 * @note  功率分配 task 必须按该周期调用，内部计时器按该周期累加。
 */
#ifndef POWER_ALLOC_2SEG_TASK_PERIOD_MS
#define POWER_ALLOC_2SEG_TASK_PERIOD_MS 100U
#endif

/**
 * @brief 接触器/电源模块控制确认超时，ms
 * @note  等待硬件反馈的状态超过该时间未确认，进入故障状态。
 */
#ifndef POWER_ALLOC_2SEG_CTRL_TIMEOUT_MS
#define POWER_ALLOC_2SEG_CTRL_TIMEOUT_MS 10000U
#endif

/**
 * @brief 两段机功率分配状态
 */
enum power_alloc_2seg_state
{
    ALLOC_2SEG_IDLE = 0,
    ALLOC_2SEG_PLAN,    /* 用于确定功率分配方案 */
    ALLOC_2SEG_WAIT_PAR,
    ALLOC_2SEG_WAIT_PDU_PWR,
    ALLOC_2SEG_WAIT_PDU_RDY,
    ALLOC_2SEG_ACTIVE,
    ALLOC_2SEG_FAULT,
    ALLOC_2SEG_RELEASE,
    ALLOC_2SEG_WAIT_VOL_MATCH,
    ALLOC_2SEG_STOP,    /* 一个公共的停机操作 */
    ALLOC_2SEG_RECONFIG,
    ALLOC_2SEG_STATE_COUNT
};

/* 停机原因 */
enum power_alloc_stop_reason
{
    STOP_REASON_IDLE,     /* 充电结束 */
    STOP_REASON_RECONFIG, /* 执行过程中发生CONN切换 */
    STOP_REASON_FAULT,    /* 执行过程中发生故障 */
};

void power_alloc_2seg_init(struct power_alloc_machine_ctx *obj);
bool power_alloc_2seg_is_in_state(const struct power_alloc_machine_ctx *machine,
                                  enum power_alloc_2seg_state state);

#ifdef __cplusplus
}
#endif

#endif /* POWER_ALLOCATION_2SEG_H */
