#ifndef BUTTON_BOUNDARY_HPP
#define BUTTON_BOUNDARY_HPP

#include "driver/gpio.h"
#include "listener.hpp"
#include "esp_attr.h"
#include "observer.hpp"
#include "esp_timer.h"
#include "esp_sleep.h"
/**
 * @class ButtonBoundary
 * @brief A class to handle button input with debounce functionality.
 */
class ButtonBoundary : public Observer {
private:
    gpio_num_t buttonPin; ///< GPIO pin for the button
    gpio_int_type_t interruptType; ///< GPIO interrupt type
    ObserverId id; ///< Button identifier
    bool lastState;       ///< Last known state of the button
    int debounceDelay;    ///< Debounce delay in milliseconds
    int64_t lastPressTime; ///< Last press timestamp
    Listener* listener; ///< Pointer to button listener
    volatile bool buttonPressed; ///< Flag set by ISR when button is pressed

public:

    /**
     * @brief Set the button listener for handling button events.
     * @param buttonListener Reference to a Listener object.
     */
    void setListener(Listener& buttonListener);

    /**
     * @brief Constructor for the ButtonBoundary class.
     * @param pin The GPIO pin number for the button.
     * @param buttonId The identifier for this button.
     * @param interruptType The GPIO interrupt type (default is GPIO_INTR_POSEDGE).
     */
    ButtonBoundary(gpio_num_t pin, ObserverId buttonId, gpio_int_type_t interruptType = GPIO_INTR_POSEDGE);

    /**
     * @brief Destructor for the ButtonBoundary class.
     */
    ~ButtonBoundary();

    /**
     * @brief Initialize the button GPIO.
     */
    void initialize();

    /**
     * @brief Check for button press events and call listener if needed.
     * Should be called periodically from main task.
     */
    void update();


    /**
     * @brief Get the button identifier.
     * @return The ButtonId associated with this button.
     */
    ObserverId getId() const override;
    
    /**
     * @brief Interrupt Service Routine (ISR) handler for GPIO events.
     * 
     * This function is executed when a GPIO interrupt is triggered. It is marked
     * with the `IRAM_ATTR` attribute to ensure it is placed in IRAM (Instruction RAM),
     * allowing it to execute quickly and reliably during interrupt handling.
     * 
     * @param arg A pointer to user-defined data passed to the ISR. This can be used
     *            to pass context or additional information to the handler.
     * 
     * @note ISRs should be kept as short as possible to avoid blocking other
     *       interrupts. Avoid using functions that are not safe to call from
     *       an ISR context, such as those that allocate memory or use the heap.
     */
     static void IRAM_ATTR gpioIsrHandler(void *arg);

    
};

#endif // BUTTON_BOUNDARY_HPP