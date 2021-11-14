#ifndef COM_H
#define COM_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "wst_common.h"

#include "logger.h"

#define RTOS_ERR_CHECK(x)   \
  do {                      \
    int retval = (x);       \
    if (retval != pdPASS) { \
      error_handler();      \
    }                       \
  } while (0)

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
void rtos_delay_ms(uint32_t ms);

extern void com_sysTickHandler(void);

#endif  // HEDWIG_H
