#include "gpioController.hpp"
#include "esp_log.h"

static const char *TAG = "GpioController";

GpioController::GpioController() 
    : gpio2(GPIO_NUM_2), gpio15(GPIO_NUM_15), initialized(false) {
}

GpioController::~GpioController() {
    // GPIO cleanup is handled by ESP-IDF when the program ends
}

esp_err_t GpioController::initialize() {
    if (initialized) {
        ESP_LOGW(TAG, "GPIO controller already initialized");
        return ESP_OK;
    }

    // Configure GPIO pins
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << gpio2) | (1ULL << gpio15);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    
    esp_err_t ret = gpio_config(&io_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure GPIO pins: %s", esp_err_to_name(ret));
        return ret;
    }

    // Set initial levels: GPIO 2 high, GPIO 15 low
    gpio_set_level(gpio2, 0);
    gpio_set_level(gpio15, 0);

    initialized = true;
    ESP_LOGI(TAG, "GPIO controller initialized successfully");
    return ESP_OK;
}

void GpioController::setHigh() {
    if (!initialized) {
        ESP_LOGE(TAG, "GPIO controller not initialized");
        return;
    }
    
    gpio_set_level(gpio2, 1);
    gpio_set_level(gpio15, 1);
    ESP_LOGD(TAG, "Set GPIO2=HIGH, GPIO15=LOW");
}

void GpioController::setLow() {
    if (!initialized) {
        ESP_LOGE(TAG, "GPIO controller not initialized");
        return;
    }
    
    gpio_set_level(gpio2, 0);
    gpio_set_level(gpio15, 0);
    ESP_LOGD(TAG, "Set GPIO2=LOW, GPIO15=HIGH");
}

void GpioController::setGpio2Level(int level) {
    if (!initialized) {
        ESP_LOGE(TAG, "GPIO controller not initialized");
        return;
    }
    
    gpio_set_level(gpio2, level);
    ESP_LOGD(TAG, "Set GPIO2=%d", level);
}

void GpioController::setGpio15Level(int level) {
    if (!initialized) {
        ESP_LOGE(TAG, "GPIO controller not initialized");
        return;
    }
    
    gpio_set_level(gpio15, level);
    ESP_LOGD(TAG, "Set GPIO15=%d", level);
}

int GpioController::getGpio2Level() {
    if (!initialized) {
        ESP_LOGE(TAG, "GPIO controller not initialized");
        return -1;
    }
    
    return gpio_get_level(gpio2);
}

int GpioController::getGpio15Level() {
    if (!initialized) {
        ESP_LOGE(TAG, "GPIO controller not initialized");
        return -1;
    }
    
    return gpio_get_level(gpio15);
}

bool GpioController::isInitialized() const {
    return initialized;
}
