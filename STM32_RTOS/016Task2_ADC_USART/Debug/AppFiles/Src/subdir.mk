################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AppFiles/Src/measurement.c \
../AppFiles/Src/process.c 

OBJS += \
./AppFiles/Src/measurement.o \
./AppFiles/Src/process.o 

C_DEPS += \
./AppFiles/Src/measurement.d \
./AppFiles/Src/process.d 


# Each subdirectory must supply rules for building sources it contributes
AppFiles/Src/%.o AppFiles/Src/%.su AppFiles/Src/%.cyclo: ../AppFiles/Src/%.c AppFiles/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -I"D:/RTOS STM Project/Energy-dashboard-with-stm32-esp32/STM32_RTOS/016Task2_ADC_USART/AppFiles" -I"D:/RTOS STM Project/Energy-dashboard-with-stm32-esp32/STM32_RTOS/016Task2_ADC_USART/AppFiles/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-AppFiles-2f-Src

clean-AppFiles-2f-Src:
	-$(RM) ./AppFiles/Src/measurement.cyclo ./AppFiles/Src/measurement.d ./AppFiles/Src/measurement.o ./AppFiles/Src/measurement.su ./AppFiles/Src/process.cyclo ./AppFiles/Src/process.d ./AppFiles/Src/process.o ./AppFiles/Src/process.su

.PHONY: clean-AppFiles-2f-Src

