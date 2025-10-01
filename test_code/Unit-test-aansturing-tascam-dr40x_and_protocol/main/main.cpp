#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_system.h"
#include "tascamBoundary.hpp"

static const char *TAG = "Unit test aansturing tascam dr40x";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello from Unit test aansturing tascam dr40x!");
    ESP_LOGI(TAG, "ESP32 chip: %s", esp_get_idf_version());
    ESP_LOGI(TAG, "Free heap: %ld bytes", esp_get_free_heap_size());
    
    // Create UART queue for Tascam communication
    QueueHandle_t uart_queue;
    
    // Initialize TascamBoundary instance
    TascamBoundary tascam(UART_NUM_2, &uart_queue);
    tascam.initialize();
    ESP_LOGI(TAG, "TascamBoundary initialized");
    
    // Test sequence starts here
    ESP_LOGI(TAG, "ESP32 in start screen - Test sequence beginning");
    
    // Wait 5 seconds, then send standby (idle) signal
    ESP_LOGI(TAG, "Waiting 5 seconds before sending standby signal...");
    vTaskDelay(pdMS_TO_TICKS(5000));
    ESP_LOGI(TAG, "Sending standby signal - Tascam should go to standby mode");
    tascam.idle();
    
    // Wait another 5 seconds, then send record signal
    ESP_LOGI(TAG, "Waiting 5 seconds before sending record signal...");
    vTaskDelay(pdMS_TO_TICKS(5000));
    ESP_LOGI(TAG, "Sending record signal - Tascam should start recording");
    tascam.startRecording();
    
    // Wait another 5 seconds, then send stop signal
    ESP_LOGI(TAG, "Waiting 5 seconds before sending stop signal...");
    vTaskDelay(pdMS_TO_TICKS(5000));
    ESP_LOGI(TAG, "Sending stop signal - Tascam should stop recording");
    tascam.stopRecording();
    
    ESP_LOGI(TAG, "Test sequence completed!");
    
    // Keep the program running and repeat the test sequence every 20 seconds
    while(1) {
        ESP_LOGI(TAG, "Waiting 10 seconds before next test cycle...");
        vTaskDelay(pdMS_TO_TICKS(10000));
        
        ESP_LOGI(TAG, "Starting next test cycle");
        
        // Repeat the sequence
        ESP_LOGI(TAG, "Sending standby signal...");
        tascam.idle();
        vTaskDelay(pdMS_TO_TICKS(5000));
        
        ESP_LOGI(TAG, "Sending record signal...");
        tascam.startRecording();
        vTaskDelay(pdMS_TO_TICKS(5000));
        
        ESP_LOGI(TAG, "Sending stop signal...");
        tascam.stopRecording();
        ESP_LOGI(TAG, "Test cycle completed!");
    }
}
