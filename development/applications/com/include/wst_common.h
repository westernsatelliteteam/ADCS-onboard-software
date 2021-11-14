#ifndef WST_COMMON_H
#define WST_COMMON_H

#include <string.h>
#include <stdio.h>

void error_handler(void);

#define WST_OK  0
#define WST_ERR -1

#define WST_ERR_CHECK(x)    \
  do {                      \
    int retval = (x);       \
    if (retval != WST_OK) { \
      error_handler();      \
    }                       \
  } while (0)

#define ASSERT(val)         \
  do {                      \
    if(!(val)) {            \
      error_handler();      \
    }                       \
  } while (0)

#endif  // WST_COMMON_H
