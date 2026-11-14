#include "app.h"
#include "main.h"
#include "cmsis_os2.h"
#include "usart.h"

#include "log.h"
LOG_MODULE_REGISTER(app, LOG_LEVEL_DBG);

osThreadId_t logTaskHandle;
const osThreadAttr_t logTask_attributes = {
  .name = "logTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 4*128
};

void LogTaskEntry(void *argument)
{

  for(;;)
  {
    LOG_ASYNC_I("log task.");
    osDelay(100);
  }
}

void app_main()
{
  log_init();
  logTaskHandle = osThreadNew(LogTaskEntry, NULL, &logTask_attributes);

  return;
}
