#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "hal/gpio_types.h"
#include "portmacro.h"
#include "soc/gpio_num.h"
#include <stdbool.h>
#include <stdint.h>

// Time in MS between two states change
#define LED_PERIOD_MS 500

// The pin number, here I use D2 pin
#define BLINK_GPIO GPIO_NUM_2

// The debug tag
static const char *TAG = "MEP-led-1";

// The default state
static uint8_t s_led_state = 0;

// Set the pin BLINK_GPIO to the s_led_state, so either 0 or 1
static void blink_led(void) { gpio_set_level(BLINK_GPIO, s_led_state); }

void app_main(void)
{
	// Set the pin BLINK_GPIO to OUTPUT mode
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_DEF_OUTPUT);
	// Start with a blink, here set the led to 0 (off)
	blink_led();
	while (1)
	{
		// Displaying the current LED state
		ESP_LOGI(TAG, "%s", s_led_state == true ? "LED ON" : "LED OFF");
		// Sent pin value associated with the state
		blink_led();
		// Set on pause the current program
		vTaskDelay(LED_PERIOD_MS / portTICK_PERIOD_MS);
		// Invert the LED state
		s_led_state = !s_led_state;
	}
}
