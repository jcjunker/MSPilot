set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)

set(CMAKE_C_FLAGS_INIT "-D__MSP432P401R__ -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wall")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,-T,${CMAKE_SOURCE_DIR}/msp432p401r.lds -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nano.specs --specs=nosys.specs")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
