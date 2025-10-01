# YOD Recorder Hardware Pin Configuration

This document describes the GPIO pin assignments and hardware connections for the YOD Recorder ESP32 project.

## GPIO Pin Assignment Table

| GPIO Pin | Hardware Component | Function | Direction | Notes |
|----------|-------------------|----------|-----------|-------|
| GPIO_2   | LED Controller    | Status LED | Output | Controlled by GpioController, External case via cable |
| GPIO_4   | Button            | Patient Button | Input | Pull-up enabled, Negative edge interrupt, External case via cable |
| GPIO_12  | Display           | Reset Pin | Output | SSD1306 OLED Display Reset |
| GPIO_15  | LED Controller    | Status LED | Output | Controlled by GpioController, External case via cable |
| GPIO_16  | UART2             | RX Pin | Input | Tascam DR-40X Communication (Inverted) |
| GPIO_17  | UART2             | TX Pin | Output | Tascam DR-40X Communication (Inverted) |
| GPIO_21  | I2C               | SCL (Clock) | I/O | Display & RTC Communication |
| GPIO_22  | I2C               | SDA (Data) | I/O | Display & RTC Communication |
| GPIO_25  | Audio Modem       | PWM Output | Output | Audio FSK Modem (23kHz/21kHz) |
| GPIO_27  | Speaker           | PWM Output | Output | Beeper/Speaker Control |
| GPIO_32  | Button            | Stop Button | Input | Pull-up enabled, Positive edge interrupt |
| GPIO_34  | Button            | Research Button | Input | Pull-up enabled, Positive edge interrupt |
| GPIO_35  | Button            | Select Button | Input | Pull-up enabled, Positive edge interrupt |

## Hardware Components Details

### Display System
- **Component**: SSD1306 OLED Display (128x64)
- **Interface**: I2C
- **Pins**: 
  - SCL: GPIO_21
  - SDA: GPIO_22
- **Address**: Default I2C address (typically 0x3C)

### Real-Time Clock (RTC)
- **Component**: DS3231 RTC Module
- **Interface**: I2C (shared with display)
- **Pins**:
  - SCL: GPIO_21
  - SDA: GPIO_22

### QR Code Scanner
- **Component**: M5Scanner
- **Interface**: I2C (shared with display and RTC)
- **Pins**:
  - SCL: GPIO_21
  - SDA: GPIO_22

### Button Interface
All buttons are configured with:
- Internal pull-up resistors enabled
- Pull-down resistors disabled
- Individual interrupt handlers

| Button | GPIO | Interrupt Type | Observer ID |
|--------|------|----------------|-------------|
| Research/Pause | GPIO_34 | Positive Edge | ResearchPause |
| Select/Start | GPIO_35 | Positive Edge | Start |
| Stop | GPIO_32 | Positive Edge | Stop |
| Patient/Pause | GPIO_4 | Negative Edge | PatientPause (External case via cable) |

### Audio System
#### Audio Modem
- **GPIO**: GPIO_25
- **Function**: FSK (Frequency Shift Keying) Modem
- **Frequencies**: 
  - High: 23,000 Hz
  - Low: 21,000 Hz
- **Implementation**: PWM-based signal generation

#### Speaker/Beeper
- **GPIO**: GPIO_27
- **Function**: Audio feedback and notifications
- **Implementation**: PWM-based tone generation

### Communication Interfaces
#### UART2 - Tascam DR-40X Control
- **Port**: UART_NUM_2
- **TX Pin**: GPIO_17
- **RX Pin**: GPIO_16
- **Configuration**:
  - Baud Rate: 9600
  - Data Bits: 8
  - Parity: Even
  - Stop Bits: 2
  - Flow Control: Disabled
  - Signal Inversion: Both TX and RX inverted

#### I2C Bus
- **Port**: I2C_NUM_0
- **SCL**: GPIO_21
- **SDA**: GPIO_22
- **Devices**: SSD1306 Display, DS3231 RTC, M5Scanner
- **Speed**: Standard 100kHz

### LED Status Indicators
- **GPIO_2**: Status LED (controlled by GpioController) - External case via cable
- **GPIO_15**: Status LED (controlled by GpioController) - External case via cable
- **Function**: Visual status indication for system states
- **Connection**: LEDs are mounted in external case and connected to ESP32 via cable

## Power and Ground Connections
- All components require 3.3V power supply
- Common ground connections for all components
- ESP32 DevKit provides regulated 3.3V output

## Pin Usage Summary
- **Total GPIO pins used**: 12 out of available ESP32 pins
- **Input pins**: 4 (buttons)
- **Output pins**: 6 (LEDs, PWM, Reset)
- **I/O pins**: 2 (I2C)
- **Communication pins**: 2 (UART)

## External Case Connections
Some components are mounted in an external case and connected to the main ESP32 board via cable:

- **Patient Button (GPIO_4)**: External momentary push button in case, connected via cable
- **Status LEDs (GPIO_2, GPIO_15)**: External indicator LEDs in case, connected via cable
- **Power (3.3V) and Ground**: The external case provides connections for 3.3V power and ground to supply the button.

## Notes
- All GPIO pins are configured with appropriate pull-up/pull-down resistors
- PWM channels use LEDC peripheral for audio generation
- UART signals are inverted for Tascam compatibility
- I2C bus is shared among multiple devices with different addresses
- Button debouncing is handled in software with interrupt-driven detection
- External case connections require proper cable management and strain relief
