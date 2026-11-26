#include "power_allocation_1seg.h"
#include "power_allocation_2seg.h"
#include "power_allocation_port_app.h"
#include "power_allocation_port_input.h"
// #include "app/storage/config/charge_config_electrical.h"
#include "power_allocation_types.h"
#include "cmsis_os2.h"
#include "Libraries/zephyr_smf/smf.h"
#include "app/logs/log.h"

LOG_MODULE_REGISTER(power_alloc);

osThreadId_t PowerAllocateHandle;
const osThreadAttr_t PowerAllocateTask_attributes = {
  .name = "PowerAllocateTask",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128 * 4
};

struct power_alloc_machine_ctx power_alloc_machine = {0};

 void power_allocation_task_entry(void *arg)
{
    (void)arg;

    POWER_ALLOCATION_SEG_TYPE_E power_alloc_type = power_allocation_get_seg_type();
    switch (power_alloc_type)
    {
    case POWER_ALLOCATION_SEG_TYPE_1SEG:
        /* code */
        break;
    case POWER_ALLOCATION_SEG_TYPE_2SEG:
        power_alloc_2seg_init(&power_alloc_machine);
        /* code */
        break;
    default:
        break;
    }

    struct power_alloc_conn_input* conn_input = &power_alloc_machine.conn[0];

    for (;;)
    {
        uint8_t req_mask = 0U;
        for(uint8_t conn_idx = 0; conn_idx < CHARGE_CONNECTOR_NUM; conn_idx++)
        {
            conn_input[conn_idx].has_req = power_alloc_conn_has_req(conn_idx);
            conn_input[conn_idx].req_valid = power_alloc_get_conn_req(conn_idx, &conn_input[conn_idx].req);
            if (conn_input[conn_idx].has_req != 0U)
            {
                req_mask = (uint8_t)(req_mask | (1U << conn_idx));
            }
        }
        power_alloc_machine.conn_masks.req_mask = req_mask;

        if (smf_run_state(SMF_CTX(&power_alloc_machine)) != 0)
        {
            break;
        }

        osDelay(10);
    }

    osThreadExit();
    return;
}

void power_allocation_init(void)
{
    PowerAllocateHandle = osThreadNew(power_allocation_task_entry, NULL, &PowerAllocateTask_attributes);
}
