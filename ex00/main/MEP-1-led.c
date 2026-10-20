#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "hal/gpio_types.h"
#include "portmacro.h"
#include "soc/gpio_num.h"
#include <stdbool.h>
#include <stdint.h>
#include <sys/_intsup.h>

// The pin number, here I use D2 pin
#define BLINK_GPIO GPIO_NUM_2

// The debug tag
static const char *TAG = "MEP-led-1";

// The default state
static uint8_t s_led_state = 0;

static void blink_led(void)
{
	// Displaying the current LED state
	ESP_LOGI(TAG, "%s", s_led_state == true ? "LED ON" : "LED OFF");
	// Set the pin BLINK_GPIO to the s_led_state, so either 0 or 1
	gpio_set_level(BLINK_GPIO, s_led_state);
}

static void mandatory(void)
{
	while (1)
	{
		// Set on pause the current program, we divide our value by the number
		// of miliseconds per ticks
		vTaskDelay(pdMS_TO_TICKS(CONFIG_LED_PERIOD_MS));
		// Sent pin value associated with the state
		blink_led();
		// Invert the LED state
		s_led_state = !s_led_state;
	}
}

static void bonus(void)
{
#ifdef CONFIG_IS_BONUS
	// The letter counter, from 0 to 8
	// 0-2 = .
	// 3-5 = -
	// 6-8 = .
	uint8_t counter = 0;

	while (1)
	{
		// Check if the letter is the O
		bool is_long = counter / 3 == 1;

		// Turning on the LED
		s_led_state = 1;
		blink_led();

		// If it is, then long bip
		if (is_long)
			vTaskDelay(pdMS_TO_TICKS(CONFIG_LED_PERIOD_MS));
		// Else, short bip
		else
			vTaskDelay(pdMS_TO_TICKS(CONFIG_LED_PERIOD_MS /
									 CONFIG_SHORT_DIVIDER_VALUE));

		// Turning off the LED
		s_led_state = 0;
		blink_led();

		// Increment s_counter, but without exceeding 8, because there is 9
		// values (0-8), and each represent a bip. SOS in morse code is
		// ...---...
		if (counter == 8)
		{
			// Marking a long pause if its the end of a SOS
			counter = 0;
			vTaskDelay(pdMS_TO_TICKS(CONFIG_SOS_PAUSE));
		}
		else
		{
			++counter;
			vTaskDelay(pdMS_TO_TICKS(CONFIG_LED_PERIOD_MS /
									 CONFIG_SHORT_DIVIDER_VALUE));
		}
	}
#endif
}

void app_main(void)
{
	// Set the pin BLINK_GPIO to OUTPUT mode
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_DEF_OUTPUT);
	// Start with a blink, here set the led to 0 (off)
	blink_led();

#ifdef CONFIG_IS_BONUS
	bonus();
#else
	mandatory();
#endif
}
