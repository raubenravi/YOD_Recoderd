#include "tascamBoundary.hpp"


TascamBoundary::TascamBoundary(uart_port_t uartNum, QueueHandle_t *uartQueue)
    : uartNum(UART_NUM_2), uartQueue(uartQueue)
{
  
}

TascamBoundary::~TascamBoundary()
{
    uart_driver_delete(uartNum);
}

void TascamBoundary::initialize()
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_EVEN,
        .stop_bits = UART_STOP_BITS_2,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        //.source_clk = UART_SCLK_DEFAULT,
        //.flags = 0
    };

    ESP_ERROR_CHECK(uart_param_config(uartNum, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uartNum, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_set_line_inverse(uartNum, UART_SIGNAL_RXD_INV | UART_SIGNAL_TXD_INV));
    ESP_ERROR_CHECK(uart_driver_install(uartNum, 256, 256, 256, uartQueue, 0));
    vTaskDelay(pdMS_TO_TICKS(100));
}



void TascamBoundary::startRecording()
{
    idle();
    vTaskDelay(pdMS_TO_TICKS(250));
    sendCommand(TascamCommands::TASCAM_START_RECORDING); // Replace with the actual command to start recording
    //sendCommand(139);
}

void TascamBoundary::sendCommand(uint8_t command)
{
    //idle(); // Ensure the device is in idle state before sending a command
    uart_write_bytes(uartNum, (const char*)&command, 1);
    vTaskDelay(pdMS_TO_TICKS(500)); // Adjust delay as needed
    
}

void TascamBoundary::idle()
{
    stopRecording();
    stopRecording();
    sendCommand(TascamCommands::TASCAM_START_RECORDING);
}

void TascamBoundary::stopRecording()
{
    sendCommand(TascamCommands::TASCAM_STOP_RECORDING); // Use the defined command to stop recording
}
