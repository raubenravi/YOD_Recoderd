// m5Scanner.cpp
#include "m5Scanner.hpp"
#include <driver/i2c_master.h>
#include <cstring>

// #define I2C_MASTER_SCL_IO 34               // GPIO number for I2C master clock
// #define I2C_MASTER_SDA_IO 35               // GPIO number for I2C master data
// #define I2C_MASTER_NUM I2C_NUM_0           // I2C port number for master dev
// #define I2C_MASTER_FREQ_HZ 100000          // I2C master clock frequency
// #define I2C_MASTER_TX_BUF_DISABLE 0        // I2C master doesn't need buffer
// #define I2C_MASTER_RX_BUF_DISABLE 0        // I2C master doesn't need buffer
#define SCANNER_ADDR 0x21                  // Example I2C address for the scanner
#define I2C_MASTER_FREQ_HZ          100000  
#define QRCODE_SCANNER_ADDR 0x21   
M5Scanner::M5Scanner(i2c_master_bus_handle_t & i2cBusHandle) : i2cBusHandle(i2cBusHandle), qrDevHandle(NULL) {
      
    // Create a device handle for the QR scanner using the global bus handle
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = QRCODE_SCANNER_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };
     i2c_master_bus_add_device(i2cBusHandle, &dev_cfg, &qrDevHandle);
}

M5Scanner::~M5Scanner() {}

void M5Scanner::initialize() {
    checkIfCodeIsScanned();
    vTaskDelay(pdMS_TO_TICKS(100));
  
}

bool M5Scanner::checkIfCodeIsScanned()
{
    uint8_t data[2];
    const uint16_t QRCODE_READY_REG = 0x0010;  // <-- Set correct register address
    qrcodeRegisterRead(QRCODE_READY_REG, data, 1);
    return data[0]  ? true : false;
}

uint8_t* M5Scanner::getCode()
{
    uint8_t data[1];
    static uint8_t qr_code_data[64]; // Use static to return pointer safely
    memset(qr_code_data, 0, sizeof(qr_code_data));
    // uint8_t qr_code_length = 0;
    const uint16_t QRCODE_LENGTH_REG = 0x0020; // <-- Set correct register address
    const uint16_t QRCODE_DATA_REG = 0x1000;   // <--  Set correct register address

    // Read the length of the QR code data
    if (qrcodeRegisterRead(QRCODE_LENGTH_REG, data, 1) != ESP_OK)
        return nullptr;
    qrCodeLengthLastScanned = data[0];


    // Read the QR code data
    if (qrCodeLengthLastScanned > 0 && qrCodeLengthLastScanned <= sizeof(qr_code_data)) {
        if (qrcodeRegisterRead(QRCODE_DATA_REG, qr_code_data, qrCodeLengthLastScanned) == ESP_OK)
            return qr_code_data;
    }
    //qrcodeRegisterRead(QRCODE_DATA_REG, qr_code_data, qr_code_length);
    return nullptr;
}

uint8_t M5Scanner::getQrCodeLength()
{
    uint8_t data[1];
    const uint16_t QRCODE_LENGTH_REG = 0x0020; // <-- Set correct register address
    if (qrcodeRegisterRead(QRCODE_LENGTH_REG, data, 1) == ESP_OK)
        return data[0];
    return 0;
}

uint8_t M5Scanner::getQrCodeLengthLastScanned() const
{
    return qrCodeLengthLastScanned;
}

esp_err_t M5Scanner::qrcodeRegisterWriteByte(uint16_t regAddr, uint8_t data)
{
    uint8_t write_buf[3] = {(uint8_t)(regAddr & 0xFF), (uint8_t)(regAddr >> 8), data};
    return i2c_master_transmit(qrDevHandle, write_buf, sizeof(write_buf), 1000 / portTICK_PERIOD_MS);
} 

esp_err_t M5Scanner::qrcodeRegisterRead(uint16_t regAddr, uint8_t *data, size_t len)
{
    uint8_t addr_bytes[2] = {(uint8_t)(regAddr & 0xFF), (uint8_t)(regAddr >> 8)};
    return i2c_master_transmit_receive(qrDevHandle, addr_bytes, 2, data, len, 1000 / portTICK_PERIOD_MS);
}

esp_err_t M5Scanner::setTriggerMode(uint8_t mode)
{
    const uint16_t QRCODE_TRIGGER_MODE_REG = 0x0030;  // <-- Set correct register address
    esp_err_t ret = qrcodeRegisterWriteByte(QRCODE_TRIGGER_MODE_REG, 1);
    
    if (ret != ESP_OK) {
        ESP_LOGE("M5Scanner", "Failed to set trigger mode: %s", esp_err_to_name(ret));
        return ret;
    }
    return ESP_OK;
}

