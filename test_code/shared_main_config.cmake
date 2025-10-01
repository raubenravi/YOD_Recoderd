# Shared main component configuration for YOD Recorder test projects
# Include this file in your test's main/CMakeLists.txt

# Common source files from the main project
set(SHARED_SRCS
    "../../../code_esp32/main/src/speaker.cpp"
    "../../../code_esp32/main/src/pwmChannel.cpp"
    "../../../code_esp32/main/src/audioAnalyzer.cpp"
    "../../../code_esp32/main/src/audioModem.cpp"
    "../../../code_esp32/main/src/buttonBoundary.cpp"
    "../../../code_esp32/main/src/displayController.cpp"
    "../../../code_esp32/main/src/ds323.cpp"
    "../../../code_esp32/main/src/gpioController.cpp"
    "../../../code_esp32/main/src/m5Scanner.cpp"
    "../../../code_esp32/main/src/m5ScannerController.cpp"
    "../../../code_esp32/main/src/menuController.cpp"
    "../../../code_esp32/main/src/RealTimeClock.cpp"
    "../../../code_esp32/main/src/storage.cpp"
    "../../../code_esp32/main/src/tascamBoundary.cpp"
    "../../../code_esp32/main/src/taskHandler.cpp"
    "../../../code_esp32/main/src/WordCountQueueObserver.cpp"
    "../../../code_esp32/main/src/adcHandler.cpp"
    "../../../code_esp32/main/src/audioAnalyzerController.cpp"
)

# Common include directories
set(SHARED_INCLUDES
    "../../../code_esp32/main/headers"
    
)
list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_SOURCE_DIR}/../../managed_components")
# Common component requirements
set(SHARED_REQUIRES
    driver 
    freertos 
    esp_common 
    log
    nvs_flash
    spiffs
    esp_timer
    esp_adc
    spi_flash
    esp-dsp
    ssd1306
)
