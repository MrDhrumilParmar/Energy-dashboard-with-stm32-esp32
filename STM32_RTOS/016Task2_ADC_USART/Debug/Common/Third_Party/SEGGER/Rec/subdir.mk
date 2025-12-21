################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/RTOS\ STM\ Project/RTOS\ Playground/Common/Third_Party/SEGGER/Rec/segger_uart.c 

OBJS += \
./Common/Third_Party/SEGGER/Rec/segger_uart.o 

C_DEPS += \
./Common/Third_Party/SEGGER/Rec/segger_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Common/Third_Party/SEGGER/Rec/segger_uart.o: D:/RTOS\ STM\ Project/RTOS\ Playground/Common/Third_Party/SEGGER/Rec/segger_uart.c Common/Third_Party/SEGGER/Rec/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -I"D:/RTOS STM Project/Energy-dashboard-with-stm32-esp32/STM32_RTOS/016Task2_ADC_USART/AppFiles" -I"D:/RTOS STM Project/Energy-dashboard-with-stm32-esp32/STM32_RTOS/016Task2_ADC_USART/AppFiles/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Common/Third_Party/SEGGER/Rec/segger_uart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Common-2f-Third_Party-2f-SEGGER-2f-Rec

clean-Common-2f-Third_Party-2f-SEGGER-2f-Rec:
	-$(RM) ./Common/Third_Party/SEGGER/Rec/segger_uart.cyclo ./Common/Third_Party/SEGGER/Rec/segger_uart.d ./Common/Third_Party/SEGGER/Rec/segger_uart.o ./Common/Third_Party/SEGGER/Rec/segger_uart.su

.PHONY: clean-Common-2f-Third_Party-2f-SEGGER-2f-Rec

