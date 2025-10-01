# YOD Recorder - ESP32 Audio Recording System

ESP32-based audio recording system for research applications with QR code scanning and external hardware control.

# Developer Responsibility

** IMPORTANT NOTICE **

This system is designed for research and development purposes. **Before using this system in real-life applications, especially in medical or research environments, developers and users must:**

1. **Think Twice Before Implementation**: Carefully evaluate the appropriateness and safety of using this system in your specific use case
2. **Stakeholder Consultation**: Always discuss implementation plans with relevant stakeholders including:
   - Medical professionals (if used in healthcare)
   - Research supervisors and ethics committees
   - IT security teams
   - Legal and compliance departments
   - End users and participants
3. **Personal Responsibility**: The developer who is using it is solely responsible for:
   - Proper system validation and testing
   - Ensuring compliance with applicable regulations
   - Risk assessment and mitigation
   - Obtaining necessary approvals and certifications
   - Training users appropriately

**This code is provided as-is without warranty. Use at your own risk and responsibility.**

## What You Need

### Hardware
- **ESP32-WROOM-32D DevKit**
- USB cable
- SSD1306 OLED Display (128x64)
- DS3231 RTC Module
- M5Scanner (QR Code Scanner)
- Tascam DR-40X Digital Recorder
- 4x Push Buttons + 2x LEDs + Speaker

## Setup ESP-IDF

**Easiest way - VS Code Extension:**
1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install [ESP-IDF Extension](https://marketplace.visualstudio.com/items?itemName=espressif.esp-idf-extension)
3. Follow setup wizard for ESP-IDF v5.0+

**Manual installation:**
- [Windows Setup Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/windows-setup.html)
- [macOS Setup Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/macos-setup.html)
- [Linux Setup Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html)

## Hardware Connections

Connect components to these ESP32 pins:

| Component | Pin | Notes |
|-----------|-----|-------|
| **Display** | | |
| SSD1306 SCL | GPIO 21 | I2C Clock |
| SSD1306 SDA | GPIO 22 | I2C Data |
| **Buttons** | | |
| Research | GPIO 34 | |
| Select | GPIO 35 | |
| Stop | GPIO 32 | |
| Patient | GPIO 4 | External case |
| **Audio** | | |
| Modem | GPIO 25 | PWM Output |
| Speaker | GPIO 27 | PWM Output |
| **LEDs** | | |
| Status LED 1 | GPIO 2 | External case |
| Status LED 2 | GPIO 15 | External case |
| **Tascam** | | |
| RX | GPIO 16 | UART |
| TX | GPIO 17 | UART |


## Quick Start

### 1. Get the Code
```bash
git clone <repository-url>
cd "YOD Recoder"
```

### 2. Setup Environment
**Windows PowerShell:**
```powershell
export IDF_PATH=~/esp/esp-idf
```

**Windows Command Prompt:**
```cmd
%IDF_PATH%\export.bat
```



### 3. Build and Flash
```bash
idf.py set-target esp32
idf.py build
idf.py -p COM3 flash
```
*Replace COM3 with your ESP32's port*

### 4. View Logs (Optional)
```bash
idf.py -p COM3 monitor
```
*Press Ctrl+] to exit*

## How It Works

### Controls
- **Green Research Button**: Start research recording
- **Yellow Research Button**: pauze recording  
- **Red Research Button**: Stop all recording
- **Patient Button**: Patient pause (external case)

### Features
- Real-time display on OLED screen
- Audio analysis
- add marks with modem communication
- QR code scanning for participant ID
- Controls Tascam DR-40X recorder 
- Status LEDs and audio feedback


## Project Structure
```
YOD Recoder/
├── code_esp32/main/    # Main application code
├── components/         # Custom components  
├── test_code/               # Unit and integration tests
└── build/             # Generated files


```