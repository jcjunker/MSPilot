#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "FreeRTOS.h"
#include "task.h"

static void ledTask(void *args);

int main(void) {
  // Set SMCLK to 12MHz
  CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);

  // Use P1.0 for LED output
  GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
  GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

  xTaskCreate(ledTask, "ledTask", configMINIMAL_STACK_SIZE, NULL,
              tskIDLE_PRIORITY + 1, NULL);

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
