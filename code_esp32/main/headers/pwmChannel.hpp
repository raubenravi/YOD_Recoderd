/**
 * @file pwmChannel.hpp
 * @brief PWM Channel control for ESP32 using LEDC peripheral
 * @author YOD Recorder Team
 * @date 2025
 */

#ifndef PWM_CHANNEL_HPP
#define PWM_CHANNEL_HPP
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/ledc.h"
#include <stdio.h>

/**
 * @brief Configuration structure for PWM channel parameters
 */
/**
 * @brief Configuration structure for PWM channel parameters
 */
struct PWMConfig {
    ledc_timer_t timer = LEDC_TIMER_0;          ///< LEDC timer selection
    ledc_mode_t mode = LEDC_LOW_SPEED_MODE;     ///< LEDC speed mode
    ledc_channel_t channel = LEDC_CHANNEL_0;    ///< LEDC channel selection
    uint32_t frequency = 10000;                 ///< Frequency in Hz (10kHz)
    uint32_t duty = 16;                         ///< Duty cycle value
    ledc_timer_bit_t resolution = LEDC_TIMER_5_BIT; ///< Timer resolution in bits
};

/**
 * @brief PWM Channel class for controlling ESP32 LEDC peripheral
 * 
 * This class provides a thread-safe interface for configuring and controlling
 * PWM channels using the ESP32's LEDC (LED Control) peripheral. It supports
 * mutex-protected operations for safe multi-threaded usage.
 */
class PWMChannel {
public:
    /**
     * @brief Constructor for PWM Channel
     * @param gpioPin GPIO pin number to be used for PWM output
     */
    PWMChannel(gpio_num_t gpioPin);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~PWMChannel();
    
    /**
     * @brief Initialize the PWM channel with configured parameters
     * @details Configures the LEDC timer and channel, then pauses the timer.
     *          This method is thread-safe and uses mutex protection.
     */
    void initialize();

protected:
    gpio_num_t gpioPin;    ///< GPIO pin number for PWM output
    PWMConfig config;      ///< PWM configuration parameters
    
    /**
     * @brief Shared mutex for thread-safe PWM operations
     * @details Static mutex shared across all PWM channel instances
     */
    static SemaphoreHandle_t pwmMutex;
    
    /**
     * @brief Create mutex if it doesn't exist
     * @details Thread-safe mutex creation with null check
     */
    static void createMutex();
};

#endif // PWM_CHANNEL_HPP
