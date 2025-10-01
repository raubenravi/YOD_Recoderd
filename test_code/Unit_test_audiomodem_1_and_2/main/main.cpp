#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "audioModem.hpp"

static const char *TAG = "Unit test audiomodem";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello from Unit test audiomodem!");
    ESP_LOGI(TAG, "ESP32 chip: %s", esp_get_idf_version());
    ESP_LOGI(TAG, "Free heap: %ld bytes", esp_get_free_heap_size());
    
    // Initialize AudioModem
    AudioModem audioModem(2100, 2300, GPIO_NUM_25);
    audioModem.initialize();
    AudioModem audioModem2(2100, 2300, GPIO_NUM_26);
    audioModem2.initialize();


    ESP_LOGI(TAG, "AudioModem initialized. Starting test loop.");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5000)); // Wait for 5 seconds
        uint8_t byteToSend = 124;
        ESP_LOGI(TAG, "Transmitting byte %d...", byteToSend);
        audioModem.transmit(byteToSend);
    }
    



}
