cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME hello_u5)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${CMAKE_CURRENT_SOURCE_DIR}/App/app.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/logs/log.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/fsm/fsm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/hsm/hsm.c

    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/elog/elog_port/elog_port.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/elog/elog_buf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/elog/elog_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/elog/elog.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/printf/printf.c

    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/zephyr_cbprintf/cbprintf_packaged.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/zephyr_cbprintf/port/cbprintf_port.c

    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/zephyr_smf/smf.c

    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/RTT/RTT/SEGGER_RTT.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/RTT/RTT/SEGGER_RTT_printf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/RTT/Syscalls/SEGGER_RTT_Syscalls_GCC.c

    ${CMAKE_CURRENT_SOURCE_DIR}/App/ccu/pwr_alloc/power_allocation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/ccu/pwr_alloc/power_allocation_2seg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/ccu/pwr_alloc/power_allocation_port_app.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/ccu/pwr_alloc/power_allocation_port_bsp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/ccu/pwr_alloc/power_allocation_port_input.c

    ${CMAKE_CURRENT_SOURCE_DIR}/App/ccu/pdu/pdu.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/App
    ${CMAKE_CURRENT_SOURCE_DIR}/App/logs
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/elog/elog_port
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/elog
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/printf
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/zephyr_cbprintf
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/zephyr_cbprintf/port
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/zephyr_cbprintf/port/compat
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/zephyr_smf
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/RTT/RTT
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/RTT/Syscalls
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/RTT/Config
    ${CMAKE_CURRENT_SOURCE_DIR}/App/fsm
    ${CMAKE_CURRENT_SOURCE_DIR}/App/hsm
    ${CMAKE_CURRENT_SOURCE_DIR}/App/ccu/pwr_alloc
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

# Convert the linked firmware to Intel HEX.
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_OBJCOPY}" -O ihex
            "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>"
            "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex"
    BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex"
    COMMENT "Generating ${CMAKE_PROJECT_NAME}.hex"
    VERBATIM
)
