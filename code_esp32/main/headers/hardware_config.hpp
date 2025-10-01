#ifndef HARDWARE_CONFIG_HPP
#define HARDWARE_CONFIG_HPP

#include "driver/gpio.h"
#include "driver/uart.h"

// Button Pins
constexpr gpio_num_t BUTTON_RESEARCH_PIN = GPIO_NUM_34;
constexpr gpio_num_t BUTTON_SELECT_PIN = GPIO_NUM_35;
constexpr gpio_num_t BUTTON_STOP_PIN = GPIO_NUM_32;
constexpr gpio_num_t BUTTON_PATIENT_PIN = GPIO_NUM_4;

// Peripheral Pins
constexpr gpio_num_t AUDIO_MODEM_PIN = GPIO_NUM_25;
constexpr gpio_num_t SPEAKER_PIN = GPIO_NUM_27;

// Tascam UART
constexpr uart_port_t TASCAM_UART_NUM = UART_NUM_2;

#endif // HARDWARE_CONFIG_HPP
