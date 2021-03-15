################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/McuLib/commonlib/src/mylibs/service/input/keypad/select/Re_keypad_stm32_hal.c 

OBJS += \
./Core/McuLib/commonlib/src/mylibs/service/input/keypad/select/Re_keypad_stm32_hal.o 

C_DEPS += \
./Core/McuLib/commonlib/src/mylibs/service/input/keypad/select/Re_keypad_stm32_hal.d 


# Each subdirectory must supply rules for building sources it contributes
Core/McuLib/commonlib/src/mylibs/service/input/keypad/select/Re_keypad_stm32_hal.o: ../Core/McuLib/commonlib/src/mylibs/service/input/keypad/select/Re_keypad_stm32_hal.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32CubeIDE/workspace/Access_control_door/Core/Myconfig" -I"D:/STM32CubeIDE/workspace/Access_control_door/Core/McuLib/commonlib/src/mylibs" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/McuLib/commonlib/src/mylibs/service/input/keypad/select/Re_keypad_stm32_hal.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

