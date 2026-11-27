#include "App/ccu/pdu/pdu.h"
#include "App/ccu/pwr_alloc/power_allocation_2seg.h"
#include "App/ccu/pwr_alloc/power_allocation_port_bsp.h"
#include "cmsis_os2.h"
#include "unity.h"

#define PDU_GROUP_COUNT 2U

struct fake_pdu
{
    bool powered;
    bool available;
    bool power_on_requested;
    uint16_t voltage_0d1v;
    uint32_t current_0d1a;
};

struct fake_pwr_alloc
{
    struct fake_pdu pdu[PDU_GROUP_COUNT];
    bool par_closed;
    bool par_close_requested;
    unsigned int output_command_count;
    uint32_t tick_ms;
};

static struct fake_pwr_alloc fake;

uint32_t osKernelGetTickCount(void)
{
    return fake.tick_ms;
}

uint32_t osKernelGetTickFreq(void)
{
    return 1000U;
}

uint8_t power_alloc_set_par_contactor(uint8_t idx, uint8_t close)
{
    TEST_ASSERT_EQUAL_UINT8(0U, idx);
    fake.par_close_requested = (close != 0U);
    return 1U;
}

uint8_t power_alloc_read_par_feedback(uint8_t idx)
{
    TEST_ASSERT_EQUAL_UINT8(0U, idx);
    return fake.par_closed;
}

bool device_pdu_get_power_state(uint8_t pdu_idx)
{
    return fake.pdu[pdu_idx].powered;
}

bool device_pdu_is_available(uint8_t pdu_idx)
{
    return fake.pdu[pdu_idx].available;
}

bool device_pdu_set_power_state(uint8_t pdu_idx, bool power_on)
{
    fake.pdu[pdu_idx].power_on_requested = power_on;
    return true;
}

bool device_pdu_set_output_vol(uint8_t pdu_idx, uint16_t target_voltage_0d1v)
{
    fake.pdu[pdu_idx].voltage_0d1v = target_voltage_0d1v;
    ++fake.output_command_count;
    return true;
}

bool device_pdu_set_output_cur(uint8_t pdu_idx, uint32_t total_current_0d1a)
{
    fake.pdu[pdu_idx].current_0d1a = total_current_0d1a;
    ++fake.output_command_count;
    return true;
}

bool device_pdu_set_current_enable(uint8_t pdu_idx, bool enable)
{
    (void)pdu_idx;
    (void)enable;
    ++fake.output_command_count;
    return true;
}

bool device_pdu_get_output_vol(uint8_t pdu_idx, uint16_t *voltage_0d1v)
{
    (void)pdu_idx;
    (void)voltage_0d1v;
    return false;
}

void setUp(void)
{
    fake = (struct fake_pwr_alloc){0};
}

void tearDown(void)
{
}

static void step(struct power_alloc_machine_ctx *machine)
{
    fake.tick_ms += 10U;
    TEST_ASSERT_EQUAL_INT32(0, smf_run_state(SMF_CTX(machine)));
}

/* IDLE状态测试用例 */
static void test_idle_state(void)
{
    struct power_alloc_machine_ctx machine = {0};
    power_alloc_2seg_init(&machine);
    step(&machine);
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_IDLE));

    setUp();
    power_alloc_2seg_init(&machine);
    fake.par_closed = 1;
    step(&machine);
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_FAULT));

    setUp();
    power_alloc_2seg_init(&machine);
    fake.pdu[0].powered = 1;
    step(&machine);
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_FAULT));

    setUp();
    power_alloc_2seg_init(&machine);
    fake.pdu[1].powered = 1;
    step(&machine);
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_FAULT));

    setUp();
    power_alloc_2seg_init(&machine);
    fake.pdu[0].powered = 1;
    fake.pdu[1].powered = 1;
    step(&machine);
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_FAULT));

    setUp();
    power_alloc_2seg_init(&machine);
    fake.pdu[0].powered = 1;
    fake.pdu[1].powered = 1;
    fake.par_closed = 1;
    step(&machine);
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_FAULT));

    setUp();
    power_alloc_2seg_init(&machine);
    const uint8_t c1 = POWER_ALLOC_2SEG_CONN1_INDEX;
    machine.req_conn_mask = (uint8_t)(1U << c1);
    step(&machine);
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_WAIT_PAR));

    setUp();
    power_alloc_2seg_init(&machine);
    const uint8_t c2 = POWER_ALLOC_2SEG_CONN1_INDEX;
    machine.req_conn_mask = (uint8_t)(1U << c2);
    step(&machine);
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_WAIT_PAR));

    setUp();
    power_alloc_2seg_init(&machine);
    machine.req_conn_mask = (uint8_t)((1U << c1) | (1U << c2));
    step(&machine);
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_WAIT_PAR));
}

static void test_wait_par_state(void)
{
    struct power_alloc_machine_ctx machine = {0};
    power_alloc_2seg_init(&machine);
    step(&machine);
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_IDLE));

    const uint8_t c1 = POWER_ALLOC_2SEG_CONN1_INDEX;
    const uint8_t c2 = POWER_ALLOC_2SEG_CONN1_INDEX;
    machine.req_conn_mask = (uint8_t)((1U << c1) | (1U << c2));
    step(&machine);
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_WAIT_PAR));
}

/* conn_1 并机输出测试用例 */
static void test_c1_parallel_waits_for_feedback_then_shares_current(void)
{
    const uint8_t c1 = POWER_ALLOC_2SEG_CONN1_INDEX;
    struct power_alloc_machine_ctx machine = {0};
    power_alloc_2seg_init(&machine);
    machine.req_conn_mask = (uint8_t)(1U << c1);
    machine.conn_pwr_reqs[c1].req_valid = true;
    machine.conn_pwr_reqs[c1].pwr_req.target_voltage_0d1v = 4000U;
    machine.conn_pwr_reqs[c1].pwr_req.target_current_0d1a = 200U;

    step(&machine); /* IDLE -> WAIT_PAR */
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_WAIT_PAR));
    TEST_ASSERT_EQUAL_HEX8(0x01U, machine.target.conn_mask);    /* 服务 C1 */
    TEST_ASSERT_TRUE(machine.target.par_closed);                /* 计划吸合 PAR */
    TEST_ASSERT_EQUAL_HEX8(0x03U, machine.target.pdu_mask);     /* 计划上电 A/B */

    step(&machine); /* PAR command sent, feedback still open */
    TEST_ASSERT_TRUE(power_alloc_2seg_is_in_state(&machine, ALLOC_2SEG_WAIT_PAR));
    TEST_ASSERT_TRUE(fake.par_close_requested);
    TEST_ASSERT_FALSE(fake.pdu[0].power_on_requested);
    TEST_ASSERT_FALSE(fake.pdu[1].power_on_requested);
    TEST_ASSERT_EQUAL_UINT(0U, fake.output_command_count);

    fake.par_closed = true;
    step(&machine); /* WAIT_PAR -> WAIT_PDU_PWR */
    const struct smf_state *wait_power = smf_get_current_leaf_state(SMF_CTX(&machine));
    step(&machine);
    TEST_ASSERT_TRUE(fake.pdu[0].power_on_requested);
    TEST_ASSERT_TRUE(fake.pdu[1].power_on_requested);

    fake.pdu[0].powered = true;
    step(&machine); /* Still waiting for group B to power on */
    TEST_ASSERT_EQUAL_PTR(wait_power, smf_get_current_leaf_state(SMF_CTX(&machine)));
    TEST_ASSERT_EQUAL_UINT(0U, fake.output_command_count);

    fake.pdu[1].powered = true;
    step(&machine); /* WAIT_PDU_PWR -> WAIT_PDU_RDY */
    fake.pdu[0].available = true;
    step(&machine); /* Still waiting for group B */
    TEST_ASSERT_EQUAL_INT(ALLOC_MODE_NONE, machine.run_mode);
    TEST_ASSERT_EQUAL_UINT(0U, fake.output_command_count);

    fake.pdu[1].available = true;
    step(&machine); /* WAIT_PDU_RDY -> ACTIVE */
    TEST_ASSERT_EQUAL_INT(ALLOC_MODE_PAR, machine.run_mode);
    step(&machine); /* Apply the charging request */

    for (uint8_t group = 0; group < PDU_GROUP_COUNT; ++group)
    {
        TEST_ASSERT_EQUAL_UINT16(4000U, fake.pdu[group].voltage_0d1v);
        TEST_ASSERT_EQUAL_UINT32(100U, fake.pdu[group].current_0d1a);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_idle_state);
    RUN_TEST(test_wait_par_state);
    RUN_TEST(test_c1_parallel_waits_for_feedback_then_shares_current);
    return UNITY_END();
}
