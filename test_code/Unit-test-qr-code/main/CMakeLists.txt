# Minimal configuration for QR code unit test
# Only include the necessary source files for QR code functionality

set(QR_CODE_SRCS
    "main.cpp"
    "../../../code_esp32/main/src/m5Scanner.cpp"
)

set(QR_CODE_INCLUDES
    "../../../code_esp32/main/headers"
)

set(QR_CODE_REQUIRES
    driver 
    freertos 
    esp_common 
    log
)

# Register the component with minimal configuration
idf_component_register(SRCS ${QR_CODE_SRCS}
                       INCLUDE_DIRS "." ${QR_CODE_INCLUDES}
                       REQUIRES ${QR_CODE_REQUIRES})
