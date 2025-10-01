#ifndef TASCAM_BOUNDARY_HPP
#define TASCAM_BOUNDARY_HPP

#include <driver/uart.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_err.h>
#include "recoderAbstract.hpp"

/**
 * @enum TascamCommands
 * @brief Commands for controlling the Tascam device
 */
enum TascamCommands : uint8_t {
    TASCAM_START_RECORDING = 107, /**< Command to start recording */
    TASCAM_STOP_RECORDING = 104,  /**< Command to stop recording */
    TASCAM_IDLE = 0               /**< Command for idle state */
};

/**
 * @class TascamBoundary
 * @brief Implements the RecoderAbstract interface for Tascam devices
 * 
 * This class provides the implementation for controlling Tascam recording devices
 * through UART communication.
 */
class TascamBoundary : public RecoderAbstract {
public:
    /**
     * @brief Constructor for TascamBoundary
     * @param uartNum The UART port number to use
     * @param uartQueue Pointer to the UART queue handle
     */
    TascamBoundary(uart_port_t uartNum, QueueHandle_t *uartQueue);
    
    /**
     * @brief Destructor for TascamBoundary
     */
    ~TascamBoundary();
    
    /**
     * @brief Initialize the Tascam device communication
     */
    void initialize();
    
    /**
     * @brief Start the recording process
     * @see RecoderAbstract::startRecording
     */
    void startRecording() override;
    
    /**
     * @brief Stop the recording process
     * @see RecoderAbstract::stopRecording
     */
    void stopRecording() override;
    
    /**
     * @brief Set the device to idle state
     * @see RecoderAbstract::idle
     */
    void idle() override;

private:
    /**
     * @brief Send a command to the Tascam device
     * @param command The command code to send
     */
    void sendCommand(uint8_t command);
    
    uart_port_t uartNum;      /**< The UART port number used for communication */
    QueueHandle_t *uartQueue; /**< Pointer to the UART queue handle */
};

#endif // TASCAM_BOUNDARY_HPP