#include "hsm.h"
#include "cmsis_os2.h"
#include "smf.h"
#include "log.h"

LOG_MODULE_REGISTER(hsm);

enum hsm_state
{
  STATE_IDLE,
  STATE_ACTIVE,
  STATE_CONNECTING,
  STATE_RUNNING,
  STATE_COUNT
};

enum hsm_event
{
  EVENT_START,
  EVENT_READY,
  EVENT_SAMPLE,
  EVENT_STOP
};

struct hsm_machine
{
  struct smf_ctx ctx; /* 必须是第一个成员。 */
  enum hsm_event event;
};

static const struct smf_state states[STATE_COUNT];

static void idle_entry(void *obj)
{
  (void)obj;
  LOG_ASYNC_I("IDLE: entry");
}

static enum smf_state_result idle_run(void *obj)
{
  struct hsm_machine *m = obj;

  if (m->event == EVENT_START)
  {
    /* 进入 ACTIVE 时，SMF 会继续进入它的初始子状态 CONNECTING。 */
    smf_set_state(SMF_CTX(m), &states[STATE_ACTIVE]);
    return SMF_EVENT_HANDLED;
  }

  return SMF_EVENT_PROPAGATE;
}

static void active_entry(void *obj)
{
  (void)obj;
  LOG_ASYNC_I("ACTIVE: entry (open shared resources)");
}

static enum smf_state_result active_run(void *obj)
{
  struct hsm_machine *m = obj;

  if (m->event == EVENT_STOP)
  {
    LOG_ASYNC_I("ACTIVE: handle STOP");
    smf_set_state(SMF_CTX(m), &states[STATE_IDLE]);
    return SMF_EVENT_HANDLED;
  }

  return SMF_EVENT_PROPAGATE;
}

static void active_exit(void *obj)
{
  (void)obj;
  LOG_ASYNC_I("ACTIVE: exit (close shared resources)");
}

static void connecting_entry(void *obj)
{
  (void)obj;
  LOG_ASYNC_I("CONNECTING: entry");
}

static enum smf_state_result connecting_run(void *obj)
{
  struct hsm_machine *m = obj;

  if (m->event == EVENT_READY)
  {
    smf_set_state(SMF_CTX(m), &states[STATE_RUNNING]);
    return SMF_EVENT_HANDLED;
  }

  return SMF_EVENT_PROPAGATE;
}

static void connecting_exit(void *obj)
{
  (void)obj;
  LOG_ASYNC_I("CONNECTING: exit");
}

static void running_entry(void *obj)
{
  (void)obj;
  LOG_ASYNC_I("RUNNING: entry");
}

static enum smf_state_result running_run(void *obj)
{
  struct hsm_machine *m = obj;

  if (m->event == EVENT_SAMPLE)
  {
    LOG_ASYNC_I("RUNNING: handle SAMPLE");
    return SMF_EVENT_HANDLED;
  }

  /* 未处理的事件（例如 STOP）交给 ACTIVE。 */
  return SMF_EVENT_PROPAGATE;
}

static void running_exit(void *obj)
{
  (void)obj;
  LOG_ASYNC_I("RUNNING: exit");
}

/* 参数顺序：entry、run、exit、parent、initial。 */
static const struct smf_state states[STATE_COUNT] = {
    [STATE_IDLE] =
        SMF_CREATE_STATE(idle_entry, idle_run, NULL, NULL, NULL),
    [STATE_ACTIVE] =
        SMF_CREATE_STATE(active_entry, active_run, active_exit,
                         NULL, &states[STATE_CONNECTING]),
    [STATE_CONNECTING] =
        SMF_CREATE_STATE(connecting_entry, connecting_run, connecting_exit,
                         &states[STATE_ACTIVE], NULL),
    [STATE_RUNNING] =
        SMF_CREATE_STATE(running_entry, running_run, running_exit,
                         &states[STATE_ACTIVE], NULL),
};

static void hsm_task(void *argument)
{
  (void)argument;
  struct hsm_machine machine = {0};
  static const enum hsm_event demo_events[] = {
      EVENT_START, EVENT_READY, EVENT_SAMPLE, EVENT_STOP};
  const size_t event_count = sizeof(demo_events) / sizeof(demo_events[0]);
  const uint32_t ticks_per_second = osKernelGetTickFreq();
  size_t next_event = 0;

  smf_set_initial(SMF_CTX(&machine), &states[STATE_IDLE]);

  for (;;)
  {
    osDelay(ticks_per_second);
    machine.event = demo_events[next_event];
    if (smf_run_state(SMF_CTX(&machine)) != 0)
    {
      break;
    }

    next_event = (next_event + 1) % event_count;
  }

  osThreadExit();
}

void hsm_init(void)
{
  static const osThreadAttr_t task_attributes = {
      .name = "hsmTask",
      .priority = osPriorityNormal,
      .stack_size = 512};

  if (osThreadNew(hsm_task, NULL, &task_attributes) == NULL)
  {
    LOG_ASYNC_E("Could not start HSM demo task");
  }
}


