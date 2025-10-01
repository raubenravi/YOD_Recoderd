# Minimal configuration for Tascam control unit test
# Include necessary source files for Tascam functionality

set(TASCAM_TEST_SRCS
    "main.cpp"
    "../../../code_esp32/main/src/tascamBoundary.cpp"
)

set(TASCAM_TEST_INCLUDES
    "../../../code_esp32/main/headers"
)

set(TASCAM_TEST_REQUIRES
    driver 
    freertos 
    esp_common 
    log
)

# Register the component with Tascam configuration
idf_component_register(SRCS ${TASCAM_TEST_SRCS}
                       INCLUDE_DIRS "." ${TASCAM_TEST_INCLUDES}
                       REQUIRES ${TASCAM_TEST_REQUIRES})
