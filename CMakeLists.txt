cmake_minimum_required(VERSION 3.5)

include(${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyusb/hw/bsp/family_support.cmake)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
family_get_project_name(PROJECT ${CMAKE_CURRENT_LIST_DIR})

message("PROJECT: ${PROJECT}")
project(${PROJECT}, C CXX ASM)

# Checks this example is valid for the family and initializes the project
family_initialize_project(${PROJECT} ${CMAKE_CURRENT_LIST_DIR})

add_executable(${PROJECT})

# source
target_sources(${PROJECT} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/midi_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/midi_gpio.c
        )

# include
target_include_directories(${PROJECT} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

# Configure compilation flags and libraries for the example... see the corresponding function
# in hw/bsp/FAMILY/family.cmake for details.
# family_configure_host_example(${PROJECT})
family_configure_target(${PROJECT})
target_link_libraries(${PROJECT} PUBLIC pico_stdlib tinyusb_host)
