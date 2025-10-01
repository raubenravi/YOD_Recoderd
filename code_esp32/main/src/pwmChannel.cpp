/**
 * @file pwmChannel.cpp
 * @brief Implementation of PWM Channel control for ESP32
 * @author YOD Recorder Team
 * @date 2025
 */

#include "pwmChannel.hpp"
#include <iostream> // For demonstration purposes (e.g., logging)

// Static mutex initialization
SemaphoreHandle_t PWMChannel::pwmMutex = NULL;

/**
 * @brief Constructor for PWM Channel
 * @param gpioPin GPIO pin number to be used for PWM output
 * 
 * Initializes the PWM channel with the specified GPIO pin and creates
 * the shared mutex if it doesn't exist.
 */
PWMChannel::PWMChannel(gpio_num_t gpioPin) 
    : gpioPin(gpioPin) {
    createMutex();
}

/**
 * @brief Destructor for PWM Channel
 * 
 * Virtual destructor for proper cleanup in inheritance scenarios.
 * Currently performs no specific cleanup operations.
 */
PWMChannel::~PWMChannel() {
}

/**
 * @brief Create shared mutex for PWM operations
 * 
 * Creates a FreeRTOS mutex if it doesn't already exist. This mutex is shared
 * across all PWM channel instances to ensure thread-safe operations.
 * 
 * @note This method is thread-safe and can be called multiple times safely.
 */
void PWMChannel::createMutex() {
    if (pwmMutex == NULL) {
        pwmMutex = xSemaphoreCreateMutex();
        if (pwmMutex == NULL) {
            std::cout << "Failed to create PWM mutex!" << std::endl;
        }
    }
}

/**
 * @brief Initialize the PWM channel with configured parameters
 * 
 * This method configures both the LEDC timer and channel according to the
 * PWMConfig settings. The initialization process includes:
 * - Timer configuration (frequency, resolution, speed mode)
 * - Channel configuration (GPIO pin, duty cycle, interrupt settings)
 * - Timer pause for controlled start
 * 
 * The operation is protected by a mutex to ensure thread safety in
 * multi-threaded environments.
 * 
 * @note The timer is initially paused after configuration. Call appropriate
 *       LEDC functions to resume operation when needed.
 * 
 * @warning If mutex acquisition fails, the initialization will be skipped
 *          and an error message will be logged.
 */
void PWMChannel::initialize() {
    // Take mutex before configuring PWM
    if (xSemaphoreTake(pwmMutex, portMAX_DELAY) == pdTRUE) {
        ledc_timer_config_t ledc_timer = {
            .speed_mode       = config.mode,
            .duty_resolution  = config.resolution,
            .timer_num        = config.timer,
            .freq_hz          = config.frequency,
            .clk_cfg          = LEDC_AUTO_CLK
        };
        ledc_timer_config(&ledc_timer);

        // LEDC Channel Configuration
        ledc_channel_config_t ledc_channel = {
            .gpio_num       = gpioPin,
            .speed_mode     = config.mode,
            .channel        = config.channel,
            .intr_type      = LEDC_INTR_DISABLE,
            .timer_sel      = config.timer,
            .duty           = config.duty,  // Set duty cycle
            .hpoint         = 0
        };
        ledc_channel_config(&ledc_channel);
        
        // Initially pause the timer
        ledc_timer_pause(config.mode, config.timer); 
        std::cout << "PWM Channel is being initialized." << std::endl;
        
        // Release mutex
        xSemaphoreGive(pwmMutex);
    } else {
        std::cout << "Failed to acquire PWM mutex for initialization!" << std::endl;
    }
}
