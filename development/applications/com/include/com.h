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

void OSSysTick_Handler(void);

#endif  // HEDWIG_H
