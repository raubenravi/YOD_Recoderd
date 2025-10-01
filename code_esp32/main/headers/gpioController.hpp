/**
 * @file gpioController.hpp
 * @brief Header file for the GpioController class.
 *
 * This class is primarily designed to control the LEDs connected to GPIO pins.
 * It provides methods to initialize the GPIO pins, set their levels, and retrieve their current states.
 */

/**
 * @class GpioController
 * @brief A class to manage GPIO pins for LED control.
 *
 * The GpioController class encapsulates the functionality to control two GPIO pins.
 * It allows setting specific levels for the pins, retrieving their current levels,
 * and checking if the controller has been initialized.
 */

#ifndef GPIO_CONTROLLER_HPP
#define GPIO_CONTROLLER_HPP

#include "driver/gpio.h"
#include "esp_err.h"

class GpioController {
private:
    gpio_num_t gpio2;
    gpio_num_t gpio15;
    bool initialized;

public:
    GpioController();
    ~GpioController();

    // Initialize the GPIO pins
    esp_err_t initialize();

    // Set GPIO 2 high and GPIO 15 low
    void setHigh();

    // Set GPIO 2 low and GPIO 15 high
    void setLow();

    // Set specific levels for each GPIO
    void setGpio2Level(int level);
    void setGpio15Level(int level);

    // Get current GPIO levels
    int getGpio2Level();
    int getGpio15Level();

    // Check if controller is initialized
    bool isInitialized() const;
};

#endif // GPIO_CONTROLLER_HPP
