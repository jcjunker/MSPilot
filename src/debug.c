#include "debug.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "uart.h"

void debugPrintf(const char *fmt, ...) {
  char str[256];
  va_list args;
  int i;

  va_start(args, fmt);
  vsprintf(str, fmt, args);
  va_end(args);

  for (i = 0; i < strlen(str); i++) {
    uartSendByte(str[i]);
  }
}
