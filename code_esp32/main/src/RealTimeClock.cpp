#include "RealTimeClock.hpp"
#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"
#include "driver/i2c_master.h"

static const char* TAG = "RealTimeClock";

#define DS3231_ADDR 0x68
#define DS3231_TIME_REG 0x00
#define DS3231_TEMP_REG 0x11

RealTimeClock::RealTimeClock(i2c_master_bus_handle_t bus_handle) : m_bus_handle(bus_handle), m_dev_handle(nullptr), m_initialized(false) {
    // Device will be initialized in initialize() method
}

esp_err_t RealTimeClock::initialize() {
    if (!m_bus_handle) {
        ESP_LOGE(TAG, "I2C bus handle is null");
        return ESP_ERR_INVALID_ARG;
    }
    
    // Create device configuration for DS3231
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = DS3231_ADDR,
        .scl_speed_hz = 400000,
    };
    
    // Add DS3231 device to the I2C bus
    esp_err_t ret = i2c_master_bus_add_device(m_bus_handle, &dev_cfg, &m_dev_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add DS3231 device to I2C bus: %s", esp_err_to_name(ret));
        return ret;
    }
    
    m_initialized = true;
    ESP_LOGI(TAG, "DS3231 RTC initialized with I2C bus handle");
    return ESP_OK;
}

bool RealTimeClock::getTemperature(float& temp) {
    if (!m_initialized) {
        ESP_LOGE(TAG, "DS3231 not initialized");
        return false;
    }
    
    uint8_t temp_data[2];
    if (readRegisters(DS3231_TEMP_REG, temp_data, 2) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read temperature registers");
        return false;
    }
    
    // Convert temperature data (DS3231 temperature format)
    int16_t temp_raw = (temp_data[0] << 8) | temp_data[1];
    temp = temp_raw / 256.0f;
    
    return true;
}

bool RealTimeClock::getTime(struct tm& time) {
    if (!m_initialized) {
        ESP_LOGE(TAG, "DS3231 not initialized");
        return false;
    }
    
    uint8_t data[7];
    if (readRegisters(DS3231_TIME_REG, data, 7) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read time registers");
        return false;
    }
    
    // Convert BCD to decimal and populate tm structure
    time.tm_sec = bcdToDec(data[0] & 0x7F);        // Seconds
    time.tm_min = bcdToDec(data[1] & 0x7F);        // Minutes
    time.tm_hour = bcdToDec(data[2] & 0x3F);       // Hours (24-hour format)
    time.tm_wday = bcdToDec(data[3] & 0x07) - 1;   // Day of week (0-6, Sunday = 0)
    time.tm_mday = bcdToDec(data[4] & 0x3F);       // Day of month
    time.tm_mon = bcdToDec(data[5] & 0x1F) - 1;    // Month (0-11)
    time.tm_year = bcdToDec(data[6]) + 100;        // Year since 1900 (assuming 21st century)
    
    return true;
}

bool RealTimeClock::setTime(const struct tm& time) {
    if (!m_initialized) {
        ESP_LOGE(TAG, "DS3231 not initialized");
        return false;
    }
    
    uint8_t data[7];
    
    // Convert decimal to BCD and prepare data
    data[0] = decToBcd(time.tm_sec);              // Seconds
    data[1] = decToBcd(time.tm_min);              // Minutes
    data[2] = decToBcd(time.tm_hour);             // Hours (24-hour format)
    data[3] = decToBcd(time.tm_wday + 1);         // Day of week (1-7)
    data[4] = decToBcd(time.tm_mday);             // Day of month
    data[5] = decToBcd(time.tm_mon + 1);          // Month (1-12)
    data[6] = decToBcd(time.tm_year % 100);       // Year (last 2 digits)
    
    // Write all time registers at once
    esp_err_t ret = ESP_OK;
    for (int i = 0; i < 7; i++) {
        ret = writeRegister(DS3231_TIME_REG + i, data[i]);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to write time register %d", i);
            return false;
        }
    }
    
    return true;
}

esp_err_t RealTimeClock::writeRegister(uint8_t reg, uint8_t data) {
    if (!m_dev_handle) {
        return ESP_ERR_INVALID_STATE;
    }
    
    uint8_t write_data[2] = {reg, data};
    esp_err_t ret = i2c_master_transmit(m_dev_handle, write_data, 2, 1000 / portTICK_PERIOD_MS);
    
    return ret;
}

esp_err_t RealTimeClock::readRegister(uint8_t reg, uint8_t* data) {
    return readRegisters(reg, data, 1);
}

esp_err_t RealTimeClock::readRegisters(uint8_t reg, uint8_t* data, size_t len) {
    if (!m_dev_handle || !data) {
        return ESP_ERR_INVALID_ARG;
    }
    
    esp_err_t ret = i2c_master_transmit_receive(m_dev_handle, &reg, 1, data, len, 1000 / portTICK_PERIOD_MS);
    
    return ret;
}

uint8_t RealTimeClock::decToBcd(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}

uint8_t RealTimeClock::bcdToDec(uint8_t val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}
