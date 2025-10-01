// m5Scanner.hpp
#ifndef M5_SCANNER_HPP
#define M5_SCANNER_HPP

#include <string>
#include <driver/i2c_types.h>
#include "driver/i2c.h"
#include "esp_log.h"
#include <driver/i2c_types.h>

/**
 * @class M5Scanner
 * @brief Class to interface with an I2C QR code scanner module for M5Stack
 * 
 * This class provides functionality to initialize and interact with a QR code scanner
 * connected via I2C bus.
 */
class M5Scanner {
    private:
    /** @brief Reference to the I2C master bus handle */
    i2c_master_bus_handle_t & i2cBusHandle;
    
    /** @brief Device handle for QR code scanner */
    i2c_master_dev_handle_t qrDevHandle;
    uint8_t qrCodeLengthLastScanned = 0; // Last scanned QR code length
    
    /**
     * @brief Write a byte to a specific register on the QR scanner
     * @param regAddr The register address to write to
     * @param data The byte to write
     * @return esp_err_t Error status
     */
    esp_err_t qrcodeRegisterWriteByte(uint16_t regAddr, uint8_t data);
    
    /**
     * @brief Read data from a specific register on the QR scanner
     * @param regAddr The register address to read from
     * @param data Pointer to buffer to store read data
     * @param len Number of bytes to read
     * @return esp_err_t Error status
     */
    esp_err_t qrcodeRegisterRead(uint16_t regAddr, uint8_t *data, size_t len);

public:
    /**
     * @brief Constructor for M5Scanner
     * @param i2cBusHandle Reference to initialized I2C bus handle
     */
    M5Scanner(i2c_master_bus_handle_t & i2cBusHandle);
    
    /**
     * @brief Destructor for M5Scanner
     */
    ~M5Scanner();
    
    /**
     * @brief Check if a code has been scanned
     * @note There appears to be a typo in this method name
     * @return true if a code has been scanned, false otherwise
     */
    bool checkIfCodeIsScanned();
    
    /**
     * @brief Scan for a QR code
     * @return Pointer to character array containing the scanned data
     */
    uint8_t* getCode();
    
    /**
     * @brief Get the length of the scanned QR code
     * @return Length of the scanned QR code as a byte
     */
    uint8_t getQrCodeLength();
    
    /**
     * @brief Get the length of the last scanned QR code
     * @return Length of the last scanned QR code as a byte
     */
    uint8_t getQrCodeLengthLastScanned() const;
    
    /**
     * @brief Initialize the QR code scanner
     */
    void initialize();
    
    /**
     * @brief Set the trigger mode for the QR code scanner
     * @param mode The trigger mode (0 = continuous, 1 = single trigger, 2 = command trigger)
     * @return esp_err_t Error status
     */
    esp_err_t setTriggerMode(uint8_t mode);
    
    /**
     * @brief Clear/forget the scanned QR code data
     * @return esp_err_t Error status
     */
  
};

#endif // M5_SCANNER_HPP