/**
 * @file speaker.hpp
 * @brief Header file for the Speaker class.
 *
 * This class is responsible for controlling a speaker using PWM signals.
 * It provides functionality to generate beep sounds at specified frequencies.
 */

#ifndef SPEAKER_HPP
#define SPEAKER_HPP
#include <stdint.h>
#include "pwmChannel.hpp"

/**
 * @class Speaker
 * @brief A class to manage speaker operations.
 *
 * The Speaker class inherits from the PWMChannel class and is designed to control
 * a speaker connected to a specific GPIO pin. It allows generating beep sounds
 * at desired frequencies.
 */
class Speaker : public PWMChannel {
public:
    Speaker(gpio_num_t gpioPin);  // Constructor
    ~Speaker(); // Destructor
    void beep(uint32_t freq); // Method to make a beep sound
};

#endif // SPEAKER_HPP
