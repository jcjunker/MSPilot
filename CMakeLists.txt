cmake_minimum_required(VERSION 3.10)
project(mspilot)

if (DEFINED ENV{MSP432_SDK_ROOT})
    set(MSP432_SDK_ROOT $ENV{MSP432_SDK_ROOT})
else()
    message(FATAL_ERROR "MSP432_SDK_ROOT environment variable not defined")
endif()

add_executable(${CMAKE_PROJECT_NAME}.out
    src/main.c
    src/startup_msp432p401r_gcc.c
    src/system_msp432p401r.c
    src/uart.c
    src/debug.c
    FreeRTOS-Kernel/croutine.c
    FreeRTOS-Kernel/event_groups.c
    FreeRTOS-Kernel/list.c
    FreeRTOS-Kernel/queue.c
    FreeRTOS-Kernel/stream_buffer.c
    FreeRTOS-Kernel/tasks.c
    FreeRTOS-Kernel/timers.c
    FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c
    FreeRTOS-Kernel/portable/MemMang/heap_3.c)

target_include_directories(${CMAKE_PROJECT_NAME}.out
    PRIVATE
        ${MSP432_SDK_ROOT}/source
        ${MSP432_SDK_ROOT}/source/third_party/CMSIS/Include
        FreeRTOS-Kernel/include
        FreeRTOS-Kernel/portable/GCC/ARM_CM4F
        src)

target_link_libraries(${CMAKE_PROJECT_NAME}.out
    ${MSP432_SDK_ROOT}/source/ti/devices/msp432p4xx/driverlib/gcc/msp432p4xx_driverlib.a)

add_custom_target(flash dslite.sh -c ${CMAKE_SOURCE_DIR}/msp432p401r.ccxml ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.out)
