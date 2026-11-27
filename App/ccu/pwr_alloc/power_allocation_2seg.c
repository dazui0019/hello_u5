/**
 * @file    power_allocation_2seg.c
 * @brief   两段功率分配状态机实现
 * @note    状态跳转条件与硬件确认动作直接写在对应状态分支内，
 *          TODO 处由后续人工补全 电源模块控制与超时处理。
 *          命名约定：模块组用 A/B，枪即连接器，用 CONNECTOR1/2 表示。
 */
#include <stdint.h>

#include "APP/ccu/pwr_alloc/power_allocation_2seg.h"
#include "App/ccu/pwr_alloc/power_allocation_port_app.h"
#include "App/ccu/pwr_alloc/power_allocation_port_bsp.h"
#include "App/ccu/pwr_alloc/power_allocation_port_input.h"
#include "Libraries/zephyr_smf/smf.h"
#include "power_allocation_types.h"
#include "App/ccu/pdu/pdu.h"
#include "app/logs/log.h"
#include "cmsis_os2.h"

LOG_MODULE_REGISTER(power_alloc_2seg);


#define CONN1_MASK (1U << POWER_ALLOC_2SEG_CONN1_INDEX)
#define CONN2_MASK (1U << POWER_ALLOC_2SEG_CONN2_INDEX)
#define CONN_BOTH (CONN1_MASK | CONN2_MASK)

#define GROUP_A 0x01U
#define GROUP_B 0x02U
#define GROUP_AB (GROUP_A | GROUP_B)
#define GROUP_COUNT 2U

static const struct smf_state states_2seg[ALLOC_2SEG_STATE_COUNT];

static void go(struct power_alloc_machine_ctx *m, enum power_alloc_2seg_state state)
{
    smf_set_state(SMF_CTX(m), &states_2seg[state]);
}

/* 记录etnry时间 */
static void timed_entry(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;
    m->state_start_tick = osKernelGetTickCount();
}

/* 检查conn连接状态是否发生改变(prepare阶段) */
static bool prep_request_changed(struct power_alloc_machine_ctx *m)
{
    if (m->req_conn_mask == m->target.conn_mask)
    {
        return false;
    }
    m->run_mode = ALLOC_MODE_NONE;
    go(m, ALLOC_2SEG_STOP);
    return true;
}

/* 本侧供电模式 */
static enum power_alloc_mode local_mode(uint8_t group)
{
    return group == GROUP_A ? ALLOC_MODE_A_ONLY : ALLOC_MODE_B_ONLY;
}

/* 关闭 group 输出 */
static void disable_group(uint8_t idx)
{
    (void)device_pdu_set_output_cur(idx, 0U);
    (void)device_pdu_set_current_enable(idx, false);
}

/* ---------- IDLE ---------- */

static enum smf_state_result idle_run(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;

    if (m->req_conn_mask != 0U)
    {
        m->target.conn_mask = m->req_conn_mask;
        m->target.par_closed = (m->req_conn_mask != CONN_BOTH);
        m->target.pdu_mask = GROUP_AB; /* 当前策略默认所有组都上电 */
        go(m, ALLOC_2SEG_WAIT_PAR);
    }

    return SMF_EVENT_HANDLED;
}

/* ---------- wait par(按需求配置并机接触器) ---------- */
// 进这个状态时保证两端压差<10V(吸合条件), 且par接触器上无电流流过
static enum smf_state_result wait_par_run(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;

    /* 检查conn连接状态是否发生改变 */
    if(prep_request_changed(m))
    {
        return SMF_EVENT_HANDLED;
    }

    // todo: 动作前不要求所有pdu关机, 但至少有一侧的输出电流为0
    // todo: 并且压差不能太大?
    (void)power_alloc_set_par_contactor(0U, m->target.par_closed);

    uint8_t par_state = power_alloc_read_par_feedback(0);
    if(par_state == m->target.par_closed)
    {
        go(m, ALLOC_2SEG_WAIT_PDU_PWR);
    }

    /* 检查超时 */
    uint32_t now_tick = osKernelGetTickCount();
    uint32_t elapsed_tick = now_tick - m->state_start_tick;
    uint32_t tick_freq = osKernelGetTickFreq();
    bool time_out = (elapsed_tick * 1000) / tick_freq > 100 ? 1 : 0;

    if(time_out)
    {
        if (m->target.par_closed != 0U)
        {
            /* 吸合超时, 改为单侧充电 */
            (void)power_alloc_set_par_contactor(0U, !(m->target.par_closed));
            m->par_close_failed = true;
            m->target.par_closed = 0U;
            m->target.pdu_mask = m->target.conn_mask;
            timed_entry(m); /* 重置超时时间 */
        }
        else
        {
            /* 断开超时 */
            //! fualt
        }
    }

    return SMF_EVENT_HANDLED;
}

/* ---------- wait pdu power up(按需求启动pdu) ---------- */

/**
 * @brief 等待交流接触器动作
 */
static enum smf_state_result wait_pdu_pwr_run(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;

    /* 检查conn连接状态是否发生改变 */
    if (prep_request_changed(m))
    {
        return SMF_EVENT_HANDLED;
    }

    /* 检查par状态 */
    uint8_t par_state = power_alloc_read_par_feedback(0);
    if(par_state != m->target.par_closed)
    {
        //! fualt
    }

    bool pdu_a_on = device_pdu_get_power_state(0U);
    bool pdu_b_on = device_pdu_get_power_state(1U);
    uint32_t pwr_mask = (uint32_t)pdu_a_on | ((uint32_t)pdu_b_on << 1U);
    if (pwr_mask == m->target.pdu_mask)
    {
        // todo: 两个pdu是否关闭, 两个pud输出电压差是否在允许范围内
        go(m, ALLOC_2SEG_WAIT_PDU_RDY);
    }

    /* 检查超时 */
    uint32_t now_tick = osKernelGetTickCount();
    uint32_t elapsed_tick = now_tick - m->state_start_tick;
    uint32_t tick_freq = osKernelGetTickFreq();
    bool time_out = (elapsed_tick * 1000) / tick_freq > 100 ? 1 : 0;
    if (time_out)
    {
        //! fualt
    }

    for (uint8_t i = 0U; i < GROUP_COUNT; ++i)
    {
        if ((m->target.pdu_mask & (1U << i)) != 0U)
        {
            (void)device_pdu_set_power_state(i, true);
        }
    }

    return SMF_EVENT_HANDLED;
}

/* ---------- wait pdu ready ---------- */

/**
 * @brief 等待PDU准备完成
 * @brief 转移条件: PDU准备完成, PDU准备超时
 */
static enum smf_state_result wait_pdu_rdy_run(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;

    if (prep_request_changed(m))
    {
        return SMF_EVENT_HANDLED;
    }

    /* 检查par状态 */
    uint8_t par_state = power_alloc_read_par_feedback(0);
    if(par_state != m->target.par_closed)
    {
        //! fualt
    }

    /* 检查超时 */
    uint32_t now_tick = osKernelGetTickCount();
    uint32_t elapsed_tick = now_tick - m->state_start_tick;
    uint32_t tick_freq = osKernelGetTickFreq();
    bool time_out = (elapsed_tick * 1000) / tick_freq > 100 ? 1 : 0;
    if (time_out)
    {
        //! fualt
    }

    /* 检查pdu可用状态(device_pdu_is_available 包含供电状态) */
    bool pdu_a = device_pdu_is_available(0U);
    bool pdu_b = device_pdu_is_available(1U);
    uint32_t feedback_mask = (uint32_t)pdu_a | ((uint32_t)pdu_b << 1U);
    if (feedback_mask == m->target.pdu_mask)
    {
        if (m->target.par_closed != 0U)
        {
            m->run_mode = ALLOC_MODE_PAR;
        }
        else
        {
            /* 非并机模式 */
            m->run_mode = m->target.pdu_mask == GROUP_AB ?
                          ALLOC_MODE_SPLIT : local_mode(m->target.pdu_mask);
        }
        go(m, ALLOC_2SEG_ACTIVE);
    }

    return SMF_EVENT_HANDLED;
}

/* ---------- active ---------- */

static enum smf_state_result active_run(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;

    /* 检查par状态 */
    uint8_t par_state = power_alloc_read_par_feedback(0);
    if(par_state != m->target.par_closed)
    {
        //! fualt
    }

    /* 检查pdu可用状态(device_pdu_is_available 包含供电状态) */
    bool pdu_a = device_pdu_is_available(0U);
    bool pdu_b = device_pdu_is_available(1U);
    uint32_t feedback_mask = (uint32_t)pdu_a | ((uint32_t)pdu_b << 1U);
    if (feedback_mask != m->target.pdu_mask)
    {
        //! fault
    }

    /* 判断是否有旧枪需要继续充电 */
    const uint8_t req_conn_mask = m->req_conn_mask;
    const uint8_t current_conn_mask = m->target.conn_mask;

    if (req_conn_mask != current_conn_mask)
    {
        const uint8_t current_pdu_mask = m->target.pdu_mask;
        const uint8_t keep_pdu_mask = current_pdu_mask & current_conn_mask & req_conn_mask;
        const uint8_t release_pdu_mask = current_pdu_mask & (uint8_t)~keep_pdu_mask;

        m->target.keep_pdu_mask = keep_pdu_mask;
        m->target.release_pdu_mask = release_pdu_mask;

        if (keep_pdu_mask == 0U)
        {
            go(m, ALLOC_2SEG_STOP);
        }
        else if (release_pdu_mask != 0U)
        {
            go(m, ALLOC_2SEG_RELEASE);
        }
        else
        {
            go(m, ALLOC_2SEG_RECONFIG);
        }

        return SMF_EVENT_HANDLED;
    }

    // todo: 这些移动到外面执行
    CHARGE_POWER_ALLOCATION_PARAM_T target_param;

    switch (m->run_mode)
    {
    case ALLOC_MODE_A_ONLY:
        /* 只有 group A 在工作 */
        target_param = m->conn_pwr_reqs[POWER_ALLOC_2SEG_CONN1_INDEX].pwr_req;
        device_pdu_set_output_vol(0, target_param.target_voltage_0d1v);
        device_pdu_set_output_cur(0, target_param.target_current_0d1a);
        break;
    case ALLOC_MODE_B_ONLY:
        /* 只有 group B 在工作 */
        target_param = m->conn_pwr_reqs[POWER_ALLOC_2SEG_CONN2_INDEX].pwr_req;
        device_pdu_set_output_vol(1, target_param.target_voltage_0d1v);
        device_pdu_set_output_cur(1, target_param.target_current_0d1a);
        break;
    case ALLOC_MODE_PAR:
        /* 并机 */
        if ((req_conn_mask & CONN1_MASK) != 0U)
        {
            target_param = m->conn_pwr_reqs[POWER_ALLOC_2SEG_CONN1_INDEX].pwr_req;
        }
        else if ((req_conn_mask & CONN2_MASK) != 0U)
        {
            target_param = m->conn_pwr_reqs[POWER_ALLOC_2SEG_CONN2_INDEX].pwr_req;
        }
        else
        {
            //todo:
        }
        device_pdu_set_output_vol(0, target_param.target_voltage_0d1v);
        device_pdu_set_output_cur(0, target_param.target_current_0d1a/2);
        device_pdu_set_output_vol(1, target_param.target_voltage_0d1v);
        device_pdu_set_output_cur(1, target_param.target_current_0d1a/2);
        break;
    case ALLOC_MODE_SPLIT:
        /* 各分支独立工作 */
        target_param = m->conn_pwr_reqs[POWER_ALLOC_2SEG_CONN1_INDEX].pwr_req;
        device_pdu_set_output_vol(0, target_param.target_voltage_0d1v);
        device_pdu_set_output_cur(0, target_param.target_current_0d1a);
        target_param = m->conn_pwr_reqs[POWER_ALLOC_2SEG_CONN2_INDEX].pwr_req;
        device_pdu_set_output_vol(1, target_param.target_voltage_0d1v);
        device_pdu_set_output_cur(1, target_param.target_current_0d1a);
        break;
    default:
        break;
    }
    device_pdu_set_current_enable(0, 1);

    return SMF_EVENT_HANDLED;
}

/* ---------- release ---------- */

/* 发零流和禁能请求 */
static bool quiesce_groups(uint8_t groups)
{
    for (uint8_t i = 0U; i < GROUP_COUNT; ++i)
    {
        if ((groups & (1U << i)) != 0U)
        {
            disable_group(i);
        }
    }

    // todo: 如果电流为0则返回true

    return false;
}

__attribute__((unused)) static bool power_off_groups(uint8_t groups)
{
    for (uint8_t i = 0U; i < GROUP_COUNT; ++i)
    {
        if ((groups & (1U << i)) != 0U)
        {
            (void)device_pdu_set_power_state(i, false);
        }
    }

    //todo: 如果已关机则返回true

    return false;
}

/* 将一侧的模块释放 */
static enum smf_state_result release_run(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;

    if (m->req_conn_mask == 0U)
    {
        m->target.keep_pdu_mask = 0U;
        m->run_mode = ALLOC_MODE_NONE;
        go(m, ALLOC_2SEG_STOP);
        return SMF_EVENT_HANDLED;
    }

    if(quiesce_groups(m->target.release_pdu_mask))
    {
        // todo: 判断 release_mask 侧输出电流是否为0
        if(true)
        {
            (void)power_alloc_set_par_contactor(0U, 0U);
        }

        uint8_t par_state = power_alloc_read_par_feedback(0);
        if(par_state == 0U)
        {
            go(m, ALLOC_2SEG_RECONFIG);
        }
    }

    // todo: 处理超时

    // todo: release完成之后, 应该设置一下充电模式(功率设置独立执行的设计下)

    return SMF_EVENT_HANDLED;
}

/* ---------- reconfig ---------- */

// 等待两侧电压匹配
static enum smf_state_result vol_match_run(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;

    if(m->target.release_pdu_mask > 0x02U)
    {
        // todo: go STOP
    }

    uint16_t target_vol = m->conn_pwr_reqs[m->target.keep_pdu_mask].pwr_req.target_voltage_0d1v;
    device_pdu_set_output_vol(m->target.release_pdu_mask, target_vol);
    device_pdu_set_output_cur(m->target.release_pdu_mask, 0U);
    device_pdu_set_current_enable(m->target.release_pdu_mask, true);

    uint16_t feedback_vol;
    device_pdu_get_output_vol(m->target.release_pdu_mask, &feedback_vol);

    // todo: 这个应该是一个范围而不是相等
    if(feedback_vol == target_vol)
    {
        go(m, ALLOC_2SEG_RECONFIG);
    }

    return SMF_EVENT_HANDLED;
}

/* ---------- reconfig ---------- */

// 如果是合并流程, 则需要保证两侧压差相同
static enum smf_state_result reconfig_run(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;

    if (m->req_conn_mask != 0U)
    {
        m->target.conn_mask = m->req_conn_mask;
        m->target.par_closed = (m->req_conn_mask != CONN_BOTH);
        m->target.pdu_mask = GROUP_AB; /* 当前策略默认所有组都上电 */
        go(m, ALLOC_2SEG_WAIT_PAR);
    }

    return SMF_EVENT_HANDLED;
}

/* ---------- stop ---------- */

static void stop_entry(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;
    (void)m;

    LOG_I("entry power_alloc_active");
}

static enum smf_state_result stop_run(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;

    if (m->req_conn_mask != 0U)
    {
        smf_set_state(SMF_CTX(m), &states_2seg[ALLOC_2SEG_WAIT_PAR]);
        return SMF_EVENT_HANDLED;
    }

    return SMF_EVENT_HANDLED;
}

static void stop_exit(void *obj)
{
    (void)obj;
    LOG_I("exit power_alloc_active");
}

/* ---------- fault ---------- */

static void fault_entry(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;
    (void)m;

    LOG_I("entry power_alloc_active");
}

static enum smf_state_result fault_run(void *obj)
{
    struct power_alloc_machine_ctx *m = obj;

    if (m->req_conn_mask != 0U)
    {
        smf_set_state(SMF_CTX(m), &states_2seg[ALLOC_2SEG_WAIT_PAR]);
    }

    return SMF_EVENT_HANDLED;
}

static void fault_exit(void *obj)
{
    (void)obj;
    LOG_I("exit power_alloc_active");
}

/* 参数顺序：entry、run、exit、parent、initial */
static const struct smf_state states_2seg[ALLOC_2SEG_STATE_COUNT] = {
    [ALLOC_2SEG_IDLE]           = SMF_CREATE_STATE(NULL, idle_run, NULL, NULL, NULL),
    [ALLOC_2SEG_WAIT_PAR]       = SMF_CREATE_STATE(timed_entry, wait_par_run, NULL, NULL, NULL),
    [ALLOC_2SEG_WAIT_PDU_PWR]   = SMF_CREATE_STATE(timed_entry, wait_pdu_pwr_run, NULL, NULL, NULL),
    [ALLOC_2SEG_WAIT_PDU_RDY]   = SMF_CREATE_STATE(timed_entry, wait_pdu_rdy_run, NULL, NULL, NULL),
    [ALLOC_2SEG_ACTIVE]         = SMF_CREATE_STATE(NULL, active_run, NULL, NULL, NULL),
    [ALLOC_2SEG_RELEASE]        = SMF_CREATE_STATE(timed_entry, release_run, NULL, NULL, NULL),
    [ALLOC_2SEG_WAIT_VOL_MATCH] = SMF_CREATE_STATE(NULL, vol_match_run, NULL, NULL, NULL),
    [ALLOC_2SEG_RECONFIG]       = SMF_CREATE_STATE(timed_entry, reconfig_run, NULL, NULL, NULL),
    [ALLOC_2SEG_STOP]           = SMF_CREATE_STATE(stop_entry, stop_run, stop_exit, NULL, NULL),
    [ALLOC_2SEG_FAULT]          = SMF_CREATE_STATE(fault_entry, fault_run, fault_exit, NULL, NULL),
};

//! RUN状态是模块可以正常执行电压电流需求的状态

void power_alloc_2seg_init(struct power_alloc_machine_ctx *obj)
{
    smf_set_initial(SMF_CTX(obj), &states_2seg[ALLOC_2SEG_IDLE]);
}

/* 判断状态机当前状态 */
bool power_alloc_2seg_is_in_state(
    const struct power_alloc_machine_ctx *machine,
    enum power_alloc_2seg_state state)
{
    return (unsigned int)state < ALLOC_2SEG_STATE_COUNT &&
           smf_get_current_leaf_state(&machine->ctx) == &states_2seg[state];
}
