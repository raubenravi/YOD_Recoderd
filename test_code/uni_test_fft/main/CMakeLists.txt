# Minimal configuration for speaker unit test
# Only include the necessary source files for speaker functionality

set(ANALYZER_SRCS
    "main.cpp"
    "../../../code_esp32/main/src/audioAnalyzer.cpp"
    "../../../code_esp32/main/src/adcHandler.cpp"
)

set(ANALYZER_INCLUDES
    "../../../code_esp32/main/headers"
)

set(ANALYZER_REQUIRES
    driver 
    freertos 
    esp_common 
    log
    esp-dsp
    esp_timer
)

# Register the component with minimal configuration
idf_component_register(SRCS ${ANALYZER_SRCS}
                       INCLUDE_DIRS "." ${ANALYZER_INCLUDES}
                       REQUIRES ${ANALYZER_REQUIRES})
