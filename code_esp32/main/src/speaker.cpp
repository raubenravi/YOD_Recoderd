#include "speaker.hpp"
#include <iostream> // For demonstration purposes (e.g., logging)

// Constructor
Speaker::Speaker(gpio_num_t gpioPin) 
    : PWMChannel(gpioPin) {
}

// Destructor
Speaker::~Speaker() {
}

// Method to make a beep sound
void Speaker::beep(uint32_t freq) {
    // Take mutex before PWM operations
    if (xSemaphoreTake(pwmMutex, portMAX_DELAY) == pdTRUE) {
        ledc_timer_resume(config.mode, config.timer);
        ledc_set_freq(config.mode, config.timer, freq);
        vTaskDelay(1500 / portTICK_PERIOD_MS); // Beep duration
        ledc_timer_pause(config.mode, config.timer);
        
        // Release mutex
        xSemaphoreGive(pwmMutex);
    } else {
        std::cout << "Failed to acquire PWM mutex for beep!" << std::endl;
    }
}
