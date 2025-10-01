#include "displayController.hpp"
#include "esp_log.h"
#include <cstring>

static const char *DISPLAY_TAG = "DisplayController";

DisplayController::DisplayController(SSD1306_t* device, QueueHandle_t wordCountQueue) 
    : display(*device), wordCountQueue(wordCountQueue) {
    ESP_LOGI(DISPLAY_TAG, "DisplayController constructed with pre-initialized device");
    // Initialize the display with proper dimensions
    ssd1306_init(&display, 128, 64);
    ESP_LOGI(DISPLAY_TAG, "Display initialized successfully");
    recording = false;
}

DisplayController::~DisplayController() {
    // Nothing to clean up specifically for this class
}

i2c_master_bus_handle_t DisplayController::getI2cBusHandle() {
    return display._i2c_bus_handle;
}

void DisplayController::clear() {
    ssd1306_clear_screen(&display, false);
    ESP_LOGD(DISPLAY_TAG, "Display cleared");
}

void DisplayController::displayText(uint8_t line, const char* text) {
    size_t len = strlen(text);
    ssd1306_display_text(&display, line, (char*)text, len, false);
    ESP_LOGD(DISPLAY_TAG, "Displayed text on line %d: %s", line, text);
}

void DisplayController::displayTextX3(uint8_t line, const char *text, bool invert) {
    if (text == nullptr) {
        ESP_LOGW(DISPLAY_TAG, "Attempted to display null text");
        return;
    }
    
    size_t len = strlen(text);
    ssd1306_display_text_x3(&display, line, (char*)text, len, invert);
    ESP_LOGD(DISPLAY_TAG, "Displayed text x3 on line %d: %s", line, text);
}

void DisplayController::displayTwoRows(const char *row1Text, const uint8_t *row2Number) {
    // Clear the screen first
    clear();
    
    // Display the first row text (typically a label/status)
    if (row1Text != nullptr) {
        displayTextX3(1, row1Text, false);
    } else {
        ESP_LOGW(DISPLAY_TAG, "First row text was null");
    }
    
    // Display the second row number if provided
    if (row2Number != nullptr) {
        char row2Buffer[8]; // Buffer for up to 255 (max uint8_t) + null terminator
        snprintf(row2Buffer, sizeof(row2Buffer), "%u", *row2Number);
        displayTextX3(5, row2Buffer, false);
        ESP_LOGD(DISPLAY_TAG, "Displayed number on second row: %u", *row2Number);
    } else {
        ESP_LOGD(DISPLAY_TAG, "No number provided for second row");
    }
}

void DisplayController::setRecording(bool isRecording) {
    recording = isRecording;
}

void DisplayController::notify(ObserverId buttonId) {
    if (buttonId == ObserverId::AudioDataAvailable) {
        // Read word count from the queue
        uint8_t wordCount = 0;
        xQueueReceive(wordCountQueue, &wordCount, 0);
        ESP_LOGI(DISPLAY_TAG, "Received word count: %u", wordCount);
        if(recording) {
            // Convert word count to percentage (max 10 words = 100%)
            uint8_t percentage = wordCount * 5;

    
            displayTwoRows("Rec...", &percentage);
            //ESP_LOGI(DISPLAY_TAG, "Displaying recording status with %u%% completion", percentage);
        }
    }
    }

