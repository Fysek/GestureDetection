################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/MCUs/STM32F429/GestureDetection/GestureDetection/Src/main.c \
D:/MCUs/STM32F429/GestureDetection/GestureDetection/Src/stm32f4xx_hal_msp.c \
D:/MCUs/STM32F429/GestureDetection/GestureDetection/Src/stm32f4xx_it.c 

OBJS += \
./Application/User/main.o \
./Application/User/stm32f4xx_hal_msp.o \
./Application/User/stm32f4xx_it.o 

C_DEPS += \
./Application/User/main.d \
./Application/User/stm32f4xx_hal_msp.d \
./Application/User/stm32f4xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/main.o: D:/MCUs/STM32F429/GestureDetection/GestureDetection/Src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F429xx -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Inc" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Drivers/STM32F4xx_HAL_Driver/Inc" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Drivers/CMSIS/Include" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f4xx_hal_msp.o: D:/MCUs/STM32F429/GestureDetection/GestureDetection/Src/stm32f4xx_hal_msp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F429xx -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Inc" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Drivers/STM32F4xx_HAL_Driver/Inc" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Drivers/CMSIS/Include" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f4xx_it.o: D:/MCUs/STM32F429/GestureDetection/GestureDetection/Src/stm32f4xx_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F429xx -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Inc" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Drivers/STM32F4xx_HAL_Driver/Inc" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Drivers/CMSIS/Include" -I"D:/MCUs/STM32F429/GestureDetection/GestureDetection/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


