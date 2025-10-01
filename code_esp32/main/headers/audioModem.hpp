#ifndef AUDIO_MODEM_HPP
#define AUDIO_MODEM_HPP
#include <stdint.h>
#include "pwmChannel.hpp"

class AudioModem : public PWMChannel {
public:
    AudioModem(uint32_t highFreq, uint32_t lowFreq, gpio_num_t gpioPin);  // Constructor
    ~AudioModem(); // Destructor
    void transmit(uint8_t data); // Method to transmit data

private:
    uint32_t highFreq; // High frequency
    uint32_t lowFreq;  // Low frequency
};

#endif // AUDIO_MODEM_HPP