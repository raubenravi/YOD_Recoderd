#ifndef DISPLAY_CONTROLLER_HPP
#define DISPLAY_CONTROLLER_HPP

#include "ssd1306.h"
#include "esp_err.h"
#include "driver/i2c_master.h"
#include "listener.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"


/**
 * @class DisplayController
 * @brief Controls the SSD1306 OLED display for showing text and simple graphics.
 *
 * This class provides a simplified interface for using the SSD1306 display,
 * with methods to display text in different sizes, clear the screen, and
 * handle common display tasks. It also inherits from Listener to handle button events.
 */
class DisplayController : public Listener {
public:
    /**
     * @brief Constructs a DisplayController object.
     * @param device Pointer to the SSD1306 device.
     * @param wordCountQueue Handle to the word count queue for reading data.
     */
    DisplayController(SSD1306_t* device, QueueHandle_t wordCountQueue);
    
    /**
     * @brief Destructor for DisplayController.
     */
    ~DisplayController();
    
    /**
     * @brief Clears the entire display.
     */
    void clear();
    
    /**
     * @brief Displays text with normal size.
     * @param line Line number (0-7 for 128x64 display).
     * @param text Text to display.
     */
    void displayText(uint8_t line, const char *text);

    /**
     * @brief Displays text with size x3.
     * @param line Line number (0-2 for 128x64 display in x3 mode).
     * @param text Text to display.
     * @param invert If true, inverts the text color.
     */
    void displayTextX3(uint8_t line, const char *text, bool invert = false);
    
    /**
     * @brief Displays a two-row layout with text and a number.
     * @param row1_text Text for the first row.
     * @param row2_number Pointer to the number for the second row.
     */
    void displayTwoRows(const char *row1_text, const uint8_t *row2_number);

    /**
     * @brief Gets the I2C bus handle.
     * @return The I2C bus handle.
     */
    i2c_master_bus_handle_t getI2cBusHandle();

    /**
     * @brief Handles button events (inherited from Listener).
     * @param buttonId The identifier of the button that was pressed.
     */
    void notify(ObserverId buttonId) override;

    void setRecording(bool isRecording);

private:
    SSD1306_t display; ///< SSD1306 display object.
    QueueHandle_t wordCountQueue; ///< Handle to the word count queue.
     bool recording;
};

#endif // DISPLAY_CONTROLLER_HPP
