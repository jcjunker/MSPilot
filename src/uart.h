#ifndef MSPILOT_UART_H
#define MSPILOT_UART_H

#include <stdint.h>

void uartInit(void);
void uartSendByte(uint8_t b);

#endif  // MSPILOT_UART_H
