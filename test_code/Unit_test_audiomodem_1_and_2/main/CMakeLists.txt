# Minimal configuration for audiomodem unit test
# Only include the necessary source files for audiomodem functionality

set(AUDIOMODEM_SRCS
    "main.cpp"
    "../../../code_esp32/main/src/audioModem.cpp"
)

set(AUDIOMODEM_INCLUDES
    "../../../code_esp32/main/headers"
)

set(AUDIOMODEM_REQUIRES
    driver 
    freertos 
    esp_common 
    log
)

# Register the component with minimal configuration
idf_component_register(SRCS ${AUDIOMODEM_SRCS}
                       INCLUDE_DIRS "." ${AUDIOMODEM_INCLUDES}
                       REQUIRES ${AUDIOMODEM_REQUIRES})
