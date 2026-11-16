#include "app.h"
#include "main.h"
#include "cmsis_os2.h"
#include "usart.h"
#include "smf.h"
#include "gpio.h"
#include "SEGGER_RTT.h"

#include "log.h"
LOG_MODULE_REGISTER(app, LOG_LEVEL_DBG);

enum demo_state
{
  STATE_IDLE,
  STATE_WORK,
  STATE_COUNT
};

/* 状态机实例：框架上下文 + 业务数据 */
struct demo_machine
{
  struct smf_ctx ctx; /* 必须放在第一个成员 */
  uint32_t run_count;
};

static const struct smf_state states[STATE_COUNT];

osThreadId_t myTaskHandle;
const osThreadAttr_t myTask_attributes = {
    .name = "myTask",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 4 * 128};

/* ---------- IDLE ---------- */

static void idle_entry(void *obj)
{
  struct demo_machine *m = obj;

  m->run_count = 0;
  LOG_ASYNC_I("IDLE: entry");
}

static enum smf_state_result idle_run(void *obj)
{
  struct demo_machine *m = obj;

  if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin))
  {
    if (++m->run_count >= 15)
    {
      smf_set_state(SMF_CTX(m), &states[STATE_WORK]);
    }
  }

  return SMF_EVENT_HANDLED;
}

static void idle_exit(void *obj)
{
  (void)obj;
  LOG_ASYNC_I("IDLE: exit");
}

/* ---------- WORK ---------- */

static void work_entry(void *obj)
{
  struct demo_machine *m = obj;

  m->run_count = 0;
  LOG_ASYNC_I("WORK: entry");
}

static enum smf_state_result work_run(void *obj)
{
  struct demo_machine *m = obj;

  if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin))
  {
    if (++m->run_count >= 15)
    {
      smf_set_state(SMF_CTX(m), &states[STATE_IDLE]);
    }
  }

  return SMF_EVENT_HANDLED;
}

static void work_exit(void *obj)
{
  (void)obj;
  LOG_ASYNC_I("WORK: exit");
}

/* 参数顺序：entry、run、exit、parent、initial */
static const struct smf_state states[STATE_COUNT] = {
    [STATE_IDLE] =
        SMF_CREATE_STATE(idle_entry, idle_run, idle_exit, NULL, NULL),

    [STATE_WORK] =
        SMF_CREATE_STATE(work_entry, work_run, work_exit, NULL, NULL),
};

void myTaskEntry(void *argument)
{
  (void)argument;
  struct demo_machine machine = {0};

  /* 初始化，并立即执行 idle_entry() */
  smf_set_initial(SMF_CTX(&machine), &states[STATE_IDLE]);

  SEGGER_RTT_printf(0, "Hello, World!\n");

  for (;;)
  {
    /* CMSIS-RTOS2 的 osDelay 单位是内核 tick */
    osDelay(10);

    if (smf_run_state(SMF_CTX(&machine)) != 0)
    {
      break;
    }
  }

  osThreadExit();
}

void app_main()
{
  log_init();
  myTaskHandle = osThreadNew(myTaskEntry, NULL, &myTask_attributes);

  return;
}
