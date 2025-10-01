#ifndef ADC_HANDLER_HPP
#define ADC_HANDLER_HPP

#include "driver/adc.h"
#include "esp_err.h"
#include "driver/adc.h"

/**
 * @brief Class for handling ADC operations
 * 
 * This class provides an interface for initializing and reading from
 * an ADC channel of the ESP32.
 */
class AdcHandler {
public:
    /**
     * @brief Construct a new ADC Handler object
     * 
     * @param channel The ADC1 channel to use
     * @param attenuation The attenuation level to set
     */
    AdcHandler(adc1_channel_t channel, adc_atten_t attenuation);
    
    /**
     * @brief Initialize the ADC
     * 
     * @return esp_err_t ESP_OK if successful, otherwise an error code
     */
    esp_err_t init();
    
    /**
     * @brief Read raw value from the ADC
     * 
     * @return int The raw ADC reading
     */
    int read_raw();

private:
    adc1_channel_t m_channel;    /**< The ADC1 channel being used */
    adc_atten_t m_attenuation;   /**< The attenuation level of the ADC */
};

#endif // ADC_HANDLER_HPP