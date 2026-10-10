#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "soc/gpio_num.h"
#include <stdio.h>

#define LED_GPIO GPIO_NUM_2

void app_main(void) {
  gpio_reset_pin(LED_GPIO);
  gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
  while (1) {
    gpio_set_level(LED_GPIO, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(LED_GPIO, 0);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
