# Third-Party Licenses and Attributions

This document lists all the frameworks and libraries used in the YOD Recorder project along with their respective licenses and attributions.

## Project License

### YOD Recorder Project
- **License**: MIT License
- **Copyright**: Copyright (c) 2025 Ruben Rajan
- **Description**: The YOD Recorder project itself is licensed under the MIT License, allowing free use, modification, and distribution.

## Main Framework

### ESP-IDF (Espressif IoT Development Framework)
- **License**: Apache License 2.0
- **Copyright**: Copyright 2015-2024 Espressif Systems (Shanghai) CO LTD
- **Repository**: https://github.com/espressif/esp-idf
- **Version**: >=4.1.0 (as specified in requirements)
- **Description**: The official development framework for ESP32 series SoCs

## Third-Party Components

### 1. ESP-DSP Library
- **Component**: `espressif/esp-dsp`
- **Version**: 1.7.0 (^1.6.4 specified)
- **License**: Apache License 2.0
- **Copyright**: Copyright Espressif Systems (Shanghai) CO LTD
- **Repository**: https://github.com/espressif/esp-dsp
- **Description**: Official DSP library for Espressif SoCs providing digital signal processing functions
- **Documentation**: https://docs.espressif.com/projects/esp-dsp/en/latest/index.html

### 2. LED Strip Library
- **Component**: `espressif/led_strip`
- **Version**: 2.4.1 (^2.4.1 specified)
- **License**: Apache License 2.0
- **Copyright**: Copyright Espressif Systems (Shanghai) CO LTD
- **Repository**: https://github.com/espressif/idf-extra-components
- **Description**: ESP-IDF component for controlling addressable LED strips

### 3. DS3231 Real-Time Clock Library
- **Component**: `esp-idf-lib/ds3231`
- **Version**: Latest (*)
- **License**: MIT License
- **Copyright**: 
  - Copyright (c) 2015 Richard A Burton <richardaburton@gmail.com>
  - Copyright (c) 2016 Bhuvanchandra DV <bhuvanchandra.dv@gmail.com>
  - Copyright (c) 2018 Ruslan V. Uss <unclerus@gmail.com>
- **Repository**: https://github.com/UncleRus/esp-idf-lib
- **Description**: ESP-IDF driver for DS3231 high precision real-time clock

### 4. I2C Device Library
- **Component**: `esp-idf-lib/i2cdev` (dependency of ds3231)
- **Version**: Managed dependency
- **License**: MIT License
- **Copyright**: Copyright (c) 2018 Ruslan V. Uss (https://github.com/UncleRus)
- **Repository**: https://github.com/UncleRus/esp-idf-lib
- **Description**: ESP-IDF I2C abstraction library

### 5. ESP-IDF-Lib Helpers
- **Component**: `esp-idf-lib/esp_idf_lib_helpers` (dependency of ds3231)
- **Version**: Managed dependency
- **License**: ISC License (BSD-style)
- **Copyright**: Copyright (c) 2019 Tomoyuki Sakurai <y@trombik.org>
- **Repository**: https://github.com/UncleRus/esp-idf-lib
- **Description**: Helper utilities for ESP-IDF-Lib components

### 6. SSD1306 OLED Display Library
- **Component**: `nopnop2002/ssd1306`
- **Version**: Latest from Git repository
- **License**: MIT License
- **Copyright**: Copyright (c) 2022 nopnop2002
- **Repository**: https://github.com/nopnop2002/esp-idf-ssd1306
- **Description**: ESP-IDF driver for SSD1306 OLED displays with I2C and SPI support



## License Compliance Notes

1. **Apache License 2.0** components (ESP-IDF, ESP-DSP, LED Strip):
   - Commercial use allowed
   - Modification allowed
   - Distribution allowed
   - Patent grant included
   - Must include license notice and copyright
   - Must state changes if modified

2. **MIT License** components (DS3231, I2CDEV, SSD1306):
   - Very permissive license
   - Commercial use allowed
   - Must include license notice and copyright
   - No warranty provided

3. **ISC License** components (ESP-IDF-Lib Helpers):
   - Similar to MIT, very permissive
   - Commercial use allowed
   - Must include license notice and copyright

4. **SSD1306 Component**:
   - Uses MIT License (permissive)
   - Commercial use allowed
   - Must include license notice and copyright


## Full License Texts

The full text of each license can be found in the respective component directories:
- Apache 2.0: `managed_components/espressif__*/LICENSE`
- MIT: `managed_components/esp-idf-lib__*/LICENSE`

For the complete license texts, refer to:
- Apache 2.0: https://www.apache.org/licenses/LICENSE-2.0
- MIT: https://opensource.org/licenses/MIT
- ISC: https://opensource.org/licenses/ISC

---
*Last updated: October 1, 2025*