#include "audioModem.hpp"
#include <iostream> // For demonstration purposes (e.g., logging)

// Constructor
AudioModem::AudioModem(uint32_t highFreq, uint32_t lowFreq, gpio_num_t gpioPin) 
    : PWMChannel(gpioPin), highFreq(highFreq), lowFreq(lowFreq) {
}

// Destructor
AudioModem::~AudioModem() {
}

// Method to transmit data
void AudioModem::transmit(uint8_t data) {
    // Take mutex before PWM operations
    if (xSemaphoreTake(pwmMutex, portMAX_DELAY) == pdTRUE) {

        // vTaskDelay(pdMS_TO_TICKS(200)); 
        // Code to transmit data using the audio modem
        // std::cout << "Transmitting data: " << static_cast<int>(data) << std::endl;
        uint8_t mask = 1; // Start with the least significant bit
        //printf("Processing data...\n");
        // ledc_set_freq(config.mode, config.timer, highFreq);
        // vTaskDelay(pdMS_TO_TICKS(100));
        ledc_timer_resume(config.mode, config.timer); // Resume PWM signal

        for (uint8_t i = 0; i < 8; i++)
        {
            //printf("Bit %d: %d\n", i, (data & mask) ? 1 : 0);
            if (data & mask)
            {
                ledc_set_freq(config.mode, config.timer, highFreq);
            }
            else
            {
                ledc_set_freq(config.mode, config.timer, lowFreq);
            }
            mask = mask << 1; // Shift the mask to the next bit
            vTaskDelay(pdMS_TO_TICKS(20)); // Delay for 100 ms
        }
        ledc_timer_pause(config.mode, config.timer); // Pause PWM signal
        
        // Release mutex
        xSemaphoreGive(pwmMutex);
    } else {
        printf("Failed to acquire PWM mutex for transmission!\n");
    }
}