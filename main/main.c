#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_pm.h"

#define LED_GPIO GPIO_NUM_2   // Onboard LED on most ESP32 dev boards
#define BLINK_PERIOD_MS 1000  // Blink every 1 second

static const char * const TAG = "BLINK_PM";

void app_main(void)
{
    ESP_LOGI(TAG, "Starting blink app with Power Management...");

    // 1. Configure Power Management (DFS + Light Sleep)
    // Set CPU to drop to 40MHz when idle, and enter light sleep when no locks are taken.
    esp_pm_config_t pm_config = {
        .max_freq_mhz = 80,          // Maximum CPU frequency (e.g., 80, 160, or 240)
        .min_freq_mhz = 40,          // Minimum CPU frequency when idle (XTAL is usually 40MHz)
        .light_sleep_enable = true   // Automatically enter light sleep when idle
    };
    ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
    ESP_LOGI(TAG, "Power management configured: 40MHz min, 80MHz max, Light sleep ON");

    // 2. Configure the LED pin as output
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    int level = 0;
    while (1) {
        level = !level;
        gpio_set_level(LED_GPIO, level);
        ESP_LOGI(TAG, "LED %s", level ? "ON" : "OFF");

        // 3. vTaskDelay blocks the task, allowing the Power Management system
        // to scale down the CPU frequency or enter Light Sleep.
        vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
    }
}
