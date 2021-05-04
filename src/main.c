#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "FreeRTOS.h"
#include "debug.h"
#include "task.h"
#include "uart.h"

static void ledTask(void *args);
static void ibusTask(void *args);

int main(void) {
  // Set SMCLK to 12MHz
  CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);

  // Use P1.0 for LED output
  GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
  GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

  uartInit();

  xTaskCreate(ledTask, "ledTask", configMINIMAL_STACK_SIZE, NULL,
              tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(ibusTask, "ibusTask", configMINIMAL_STACK_SIZE, NULL,
              tskIDLE_PRIORITY + 2, NULL);

  vTaskStartScheduler();

  for (;;)
    ;

  return 0;
}

static void ledTask(void *args) {
  for (;;) {
    // Toggle LED every second
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

static void ibusTask(void *args) {
  uint8_t b;
  int byte_ctr = 0;
  uint8_t pkt[32];
  uint16_t chans[4];

  for (;;) {
    b = uartReceiveByte();
    pkt[byte_ctr++] = b;

    if (byte_ctr == 32) {
      chans[0] = (uint16_t)(pkt[2] | pkt[3] << 8);
      chans[1] = (uint16_t)(pkt[4] | pkt[5] << 8);
      chans[2] = (uint16_t)(pkt[6] | pkt[7] << 8);
      chans[3] = (uint16_t)(pkt[8] | pkt[9] << 8);

      byte_ctr = 0;
    }
  }
}
