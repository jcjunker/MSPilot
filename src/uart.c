#include "uart.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "FreeRTOS.h"
#include "queue.h"

static QueueHandle_t tx_queue;
static QueueHandle_t rx_queue;

void uartInit(void) {
  eUSCI_UART_ConfigV1 uart_config = {
      EUSCI_A_UART_CLOCKSOURCE_SMCLK,
      6,
      8,
      32,
      EUSCI_A_UART_NO_PARITY,
      EUSCI_A_UART_LSB_FIRST,
      EUSCI_A_UART_ONE_STOP_BIT,
      EUSCI_A_UART_MODE,
      EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
      EUSCI_A_UART_8_BIT_LEN};

  tx_queue = xQueueCreate(128, sizeof(uint8_t));

  GPIO_setAsPeripheralModuleFunctionInputPin(
      GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

  // Configure UART0 for 115200 baud
  UART_initModule(EUSCI_A0_BASE, &uart_config);
  UART_enableModule(EUSCI_A0_BASE);

  Interrupt_setPriority(INT_EUSCIA0, configKERNEL_INTERRUPT_PRIORITY);
  Interrupt_enableInterrupt(INT_EUSCIA0);

  rx_queue = xQueueCreate(128, sizeof(uint8_t));

  GPIO_setAsPeripheralModuleFunctionInputPin(
      GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

  // Configure UART2 for 115200 baud
  UART_initModule(EUSCI_A2_BASE, &uart_config);
  UART_enableModule(EUSCI_A2_BASE);

  UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);

  Interrupt_setPriority(INT_EUSCIA2, configKERNEL_INTERRUPT_PRIORITY);
  Interrupt_enableInterrupt(INT_EUSCIA2);
}

void uartSendByte(uint8_t b) {
  xQueueSend(tx_queue, &b, portMAX_DELAY);
  UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
}

uint8_t uartReceiveByte(void) {
  uint8_t b;

  xQueueReceive(rx_queue, &b, portMAX_DELAY);
  return b;
}

void EUSCIA0_IRQHandler(void) {
  uint32_t status;
  uint8_t b;
  BaseType_t task_woken = pdFALSE;

  status = UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

  if (status & EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) {
    if (xQueueReceiveFromISR(tx_queue, &b, &task_woken)) {
      UART_transmitData(EUSCI_A0_BASE, b);
    } else {
      UART_disableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
    }
  }

  portYIELD_FROM_ISR(task_woken);
}

void EUSCIA2_IRQHandler(void) {
  uint32_t status;
  uint8_t b;
  BaseType_t task_woken = pdFALSE;

  status = UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

  if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) {
    b = UART_receiveData(EUSCI_A2_BASE);
    xQueueSendFromISR(rx_queue, &b, &task_woken);
  }

  portYIELD_FROM_ISR(task_woken);
}
