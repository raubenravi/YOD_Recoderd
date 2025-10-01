#include "adcHandler.hpp"
#include "esp_log.h"

static const char *TAG = "AdcHandler";

AdcHandler::AdcHandler(adc1_channel_t channel, adc_atten_t attenuation)
    : m_channel(channel), m_attenuation(attenuation) {}

esp_err_t AdcHandler::init() {
    esp_err_t ret;

    ret = adc1_config_width(ADC_WIDTH_BIT_12);
    ret = adc1_config_channel_atten(m_channel, m_attenuation);
    ESP_LOGI(TAG, "ADC initialized successfully for channel %d", m_channel);
    return ESP_OK;
}

int AdcHandler::read_raw() {
    return adc1_get_raw(m_channel);
}