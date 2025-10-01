#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "audioAnalyzer.hpp"
#include "esp_timer.h"

static const char *TAG = "Unit test Audio Analyzer";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello from Unit test Audio Analyzer!");
    ESP_LOGI(TAG, "ESP32 chip: %s", esp_get_idf_version());
    ESP_LOGI(TAG, "Free heap: %ld bytes", esp_get_free_heap_size());
    
    uint8_t count = 0;
    uint8_t i = 0;
    TickType_t lastWakeTime = xTaskGetTickCount(); // Initialize lastWakeTime

    
    AudioAnalyzer audioAnalyzer;
    audioAnalyzer.init();    

    int64_t startTime = 0;
    uint8_t consecutiveWords = 0;
    TickType_t lastLogTime = xTaskGetTickCount();

    ESP_LOGI(TAG, "Audio analysis loop started.");
    while (1) {
        if (i == 0) {
            startTime = esp_timer_get_time();
        }
        if ((xTaskGetTickCount() - lastWakeTime) >= pdMS_TO_TICKS(250)) {
            lastWakeTime = xTaskGetTickCount();
            audioAnalyzer.sampleInput();
            audioAnalyzer.computeFft();
            // audioAnalyzer.printResults();
            if(audioAnalyzer.isWord()){
                consecutiveWords++;
            } else {
                consecutiveWords = 0;
            }

            if(consecutiveWords == 2){
                count++;
                consecutiveWords = 0; // Reset after counting
            }
             // Update lastWakeTime after processing
            i += 1;
        }

        // Log ratio every 10 seconds
        if ((xTaskGetTickCount() - lastLogTime) >= pdMS_TO_TICKS(10000)) {
            float currentRatio = (i > 0) ? (float)count / i * 2 : 0;
            ESP_LOGI(TAG, "Intermediate Ratio: %.2f after %d samples", currentRatio, i);
            lastLogTime = xTaskGetTickCount();
        }

        if (i >= 240) {
            int64_t endTime = esp_timer_get_time();
            float ratio = (i > 0) ? (float)count / i * 2 : 0;
            ESP_LOGI(TAG, "Analysis cycle complete. Count = %d, Samples = %d, Ratio = %.2f, Time = %lld ms", count, i, ratio, (endTime - startTime) / 1000);
            i = 0;
            count = 0;
            consecutiveWords = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks
    }
}
