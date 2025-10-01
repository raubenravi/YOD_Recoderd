#include "buttonBoundary.hpp"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


ButtonBoundary::ButtonBoundary(gpio_num_t pin, ObserverId buttonId, gpio_int_type_t interruptType)
    : buttonPin(pin), interruptType(interruptType), id(buttonId), listener(nullptr), buttonPressed(false)
{
}

ButtonBoundary::~ButtonBoundary()
{
    // Remove the ISR handler when the button is destroyed
    gpio_isr_handler_remove(buttonPin);
}

void ButtonBoundary::setListener(Listener& buttonListener)
{
    listener = &buttonListener;
}

void ButtonBoundary::initialize()
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = interruptType; // Use the interrupt type from constructor
    io_conf.pin_bit_mask = (1ULL << buttonPin);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_conf);
    
    // Short delay to ensure GPIO configuration is complete
   // Add the ISR handler for this button
    gpio_isr_handler_add(buttonPin, gpioIsrHandler, this);
    vTaskDelay(pdMS_TO_TICKS(200));

    // Add the ISR handler for this specific button pin
    // esp_err_t ret = gpio_isr_handler_add(buttonPin, gpioIsrHandler, this);
    // if (ret != ESP_OK) {
    //     printf("Failed to add ISR handler for GPIO %d: %s\n", buttonPin, esp_err_to_name(ret));
    //     return;
    // }
    
    // printf("Button initialized on GPIO %d with interrupt type %d\n", buttonPin, interruptType);
    
    // Small delay to ensure ISR handler is properly registered
    vTaskDelay(pdMS_TO_TICKS(50));

}


void ButtonBoundary::update() {
    if (buttonPressed) {
        buttonPressed = false; // Clear the flag
        if (listener != nullptr) {
            listener->notify(id); // Pass the button ID to the listener
        }
    }
}



ObserverId ButtonBoundary::getId() const {
    return id;
}

void IRAM_ATTR ButtonBoundary::gpioIsrHandler(void *arg) {
    // ISR should be kept minimal - just set a flag
    ButtonBoundary *button = static_cast<ButtonBoundary *>(arg);
    button->buttonPressed = true; 
}