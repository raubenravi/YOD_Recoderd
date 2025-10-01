#ifndef RTC_CLOCK_BOUNDARY_HPP
#define RTC_CLOCK_BOUNDARY_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ctime>
#include "esp_err.h"
#include "driver/i2c_master.h"

/**
 * @file RealTimeClock.hpp
 * @brief Real-time clock interface for DS3231 RTC module
 * @author //based on https://github.com/esp-idf-lib/ds3231 
 * @date 2025
 */

/**
 * @class RealTimeClock
 * @brief A class to interface with DS3231 Real-Time Clock module over I2C
 * 
 * it`s based on https://github.com/esp-idf-lib/ds3231  but the i2c driver has been replaced
 * 
 * This class provides functionality to read and write time/date information
 * from a DS3231 RTC module, as well as read temperature data from the built-in
 * temperature sensor.
 * 
 * @note Requires ESP-IDF I2C master driver
 */
class RealTimeClock {
public:
    /**
     * @brief Constructor for RealTimeClock
     * @param bus_handle I2C master bus handle for communication
     * @todo Move parameters from init to constructor and check device handle
     */
    RealTimeClock(i2c_master_bus_handle_t bus_handle);
    
    /**
     * @brief Initialize the RTC device
     * @return ESP_OK on success, error code on failure
     * @retval ESP_OK Initialization successful
     * @retval ESP_FAIL Initialization failed
     */
    esp_err_t initialize();
    
    /**
     * @brief Read temperature from the RTC's built-in temperature sensor
     * @param[out] temp Reference to store the temperature value in Celsius
     * @return true if temperature read successfully, false otherwise
     * @note Temperature resolution is typically 0.25°C
     */
    bool getTemperature(float& temp);
    
    /**
     * @brief Read current time from the RTC
     * @param[out] time Reference to tm structure to store the current time
     * @return true if time read successfully, false otherwise
     * @note The tm structure follows standard C library format
     */
    bool getTime(struct tm& time);
    
    /**
     * @brief Set the current time on the RTC
     * @param[in] time Reference to tm structure containing the time to set
     * @return true if time set successfully, false otherwise
     * @note The tm structure should follow standard C library format
     */
    bool setTime(const struct tm& time);
    
private:
    i2c_master_bus_handle_t m_bus_handle;  ///< I2C master bus handle
    i2c_master_dev_handle_t m_dev_handle;  ///< I2C device handle for RTC
    bool m_initialized;                     ///< Initialization status flag
    
    /**
     * @brief Write data to a specific register on the RTC
     * @param reg Register address to write to
     * @param data Data byte to write
     * @return ESP_OK on success, error code on failure
     */
    esp_err_t writeRegister(uint8_t reg, uint8_t data);
    
    /**
     * @brief Read data from a specific register on the RTC
     * @param reg Register address to read from
     * @param[out] data Pointer to store the read data
     * @return ESP_OK on success, error code on failure
     */
    esp_err_t readRegister(uint8_t reg, uint8_t* data);
    
    /**
     * @brief Read multiple bytes from consecutive registers on the RTC
     * @param reg Starting register address
     * @param[out] data Pointer to buffer to store the read data
     * @param len Number of bytes to read
     * @return ESP_OK on success, error code on failure
     */
    esp_err_t readRegisters(uint8_t reg, uint8_t* data, size_t len);
    
    /**
     * @brief Convert decimal value to Binary-Coded Decimal (BCD)
     * @param val Decimal value to convert (0-99)
     * @return BCD representation of the input value
     * @note Used for time/date values in RTC registers
     */
    uint8_t decToBcd(uint8_t val);
    
    /**
     * @brief Convert Binary-Coded Decimal (BCD) to decimal value
     * @param val BCD value to convert
     * @return Decimal representation of the input value
     * @note Used for time/date values from RTC registers
     */
    uint8_t bcdToDec(uint8_t val);
};


#endif // RTC_CLOCK_BOUNDARY_HPP
