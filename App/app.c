#include "app.h"
#include "main.h"
#include "cmsis_os2.h"
#include "usart.h"
#include "smf.h"
#include "gpio.h"
#include "SEGGER_RTT.h"
#include "fsm.h"

#include "log.h"
LOG_MODULE_REGISTER(app, LOG_LEVEL_DBG);

void app_main()
{
  log_init();
  fsm_init();

  return;
}
