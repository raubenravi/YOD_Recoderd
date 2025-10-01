#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/i2c_master.h"
#include "m5Scanner.hpp"
#include <string>

static const char *TAG = "Unit test qr code";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello from Unit test qr code!");
    ESP_LOGI(TAG, "ESP32 chip: %s", esp_get_idf_version());
    ESP_LOGI(TAG, "Free heap: %ld bytes", esp_get_free_heap_size());
    
    // Initialize I2C bus for scanner
       i2c_master_bus_config_t i2c_mst_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = GPIO_NUM_22,  // Adjust according to your setup
        .scl_io_num = GPIO_NUM_21,  // Adjust according to your setup
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags = {
            .enable_internal_pullup = true,
        },
    }; 
    
    i2c_master_bus_handle_t i2c_bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &i2c_bus_handle));
    
    // Initialize M5Scanner
    M5Scanner scanner(i2c_bus_handle);
    scanner.initialize();
    
    ESP_LOGI(TAG, "M5Scanner initialized");
    
    while(1) {
        // Check if QR code is scanned and get result
        if (scanner.checkIfCodeIsScanned()) {
            uint8_t* qr_data = scanner.getCode();
            uint8_t qr_length = scanner.getQrCodeLength();
            
            if (qr_data != nullptr && qr_length > 0) {
                // Convert to string for logging
                std::string qr_result((char*)qr_data, qr_length);
                ESP_LOGI(TAG, "QR Code detected: %s (length: %d)", qr_result.c_str(), qr_length);
            } else {
                ESP_LOGI(TAG, "QR Code detected but data is invalid");
            }
        } else {
            ESP_LOGI(TAG, "No QR Code detected");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
