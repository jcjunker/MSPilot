#ifndef MSPILOT_UART_H
#define MSPILOT_UART_H

#include <stdint.h>

void uartInit(void);
void uartSendByte(uint8_t b);
uint8_t uartReceiveByte(void);

#endif  // MSPILOT_UART_H
