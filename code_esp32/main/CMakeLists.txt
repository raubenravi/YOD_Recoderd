idf_component_register(SRCS 
    "main.cpp"
    "src/adcHandler.cpp"
    "src/tascamBoundary.cpp"
    "src/buttonBoundary.cpp"
    "src/menuController.cpp"
    "src/m5Scanner.cpp"
    "src/storage.cpp"
    "src/audioAnalyzer.cpp"
    "src/audioModem.cpp"
    "src/displayController.cpp"
    "src/m5ScannerController.cpp"
    "src/speaker.cpp"
    "src/pwmChannel.cpp"
    "src/RealTimeClock.cpp"

    "src/taskHandler.cpp"
    "src/WordCountQueueObserver.cpp"
    "src/gpioController.cpp"

    INCLUDE_DIRS "." ".." "src" "headers"
    REQUIRES esp-dsp
    esp_adc
    esp_system
    log
    heap
    esp_rom
    driver
    ssd1306
    nvs_flash
    esp_wifi
    esp_event
    esp_http_client
    esp_https_ota
    esp_partition
    esp_timer
    esp_app_format
    esp_common
    mbedtls	
	led_strip

)
