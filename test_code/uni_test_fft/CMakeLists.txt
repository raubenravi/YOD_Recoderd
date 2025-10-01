# Set minimum CMake version first (required)
cmake_minimum_required(VERSION 3.16)

# Include shared configuration
include(../shared_main_config.cmake)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Define the project
project(unit_test_fft)
