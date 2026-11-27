cmake_minimum_required(VERSION 3.22)
project(hello_u5_host_tests LANGUAGES C)

enable_testing()

set(firmware_dir "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(unity_dir "${CMAKE_CURRENT_SOURCE_DIR}/third_party/Unity/src")

add_executable(test_power_allocation_2seg
    test_power_allocation_2seg.c
    "${firmware_dir}/App/ccu/pwr_alloc/power_allocation_2seg.c"
    "${firmware_dir}/Libraries/zephyr_smf/smf.c"
    "${unity_dir}/unity.c"
)

target_compile_features(test_power_allocation_2seg PRIVATE c_std_11)
target_compile_definitions(test_power_allocation_2seg PRIVATE CCU_LOG_DEFAULT_LEVEL=0)
target_include_directories(test_power_allocation_2seg PRIVATE
    "${firmware_dir}"
    "${firmware_dir}/App/logs"
    "${firmware_dir}/Libraries/elog"
    "${firmware_dir}/Libraries/elog/elog_port"
    "${firmware_dir}/Middlewares/Third_Party/CMSIS/RTOS2/Include"
    "${unity_dir}"
)

add_test(NAME power_allocation_2seg COMMAND test_power_allocation_2seg)
