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
    
    // Initialize AudioAnalyzer
    AudioAnalyzer audioAnalyzer;
    if (audioAnalyzer.init() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize AudioAnalyzer. Halting.");
        return;
    }

    ESP_LOGI(TAG, "AudioAnalyzer initialized. Starting test loop.");

    while(1) {
        ESP_LOGI(TAG, "Sampling and analyzing audio...");
        audioAnalyzer.sampleInput();
        int64_t start_time = esp_timer_get_time();
        audioAnalyzer.computeFft();
        int64_t end_time = esp_timer_get_time();
        ESP_LOGI(TAG, "computeFft() took %lld us", (end_time - start_time));
        audioAnalyzer.printResults();
        vTaskDelay(pdMS_TO_TICKS(5000)); 
    }
}
