################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/RTOS\ STM\ Project/RTOS\ Playground/Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT.c \
D:/RTOS\ STM\ Project/RTOS\ Playground/Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.c \
D:/RTOS\ STM\ Project/RTOS\ Playground/Common/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.c 

S_UPPER_SRCS += \
D:/RTOS\ STM\ Project/RTOS\ Playground/Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT.o \
./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o \
./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.o \
./Common/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.o 

S_UPPER_DEPS += \
./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT.d \
./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.d \
./Common/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT.o: D:/RTOS\ STM\ Project/RTOS\ Playground/Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT.c Common/Third_Party/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -I"D:/RTOS STM Project/Energy-dashboard-with-stm32-esp32/STM32_RTOS/016Task2_ADC_USART/AppFiles" -I"D:/RTOS STM Project/Energy-dashboard-with-stm32-esp32/STM32_RTOS/016Task2_ADC_USART/AppFiles/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o: D:/RTOS\ STM\ Project/RTOS\ Playground/Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S Common/Third_Party/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -x assembler-with-cpp -MMD -MP -MF"Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.o: D:/RTOS\ STM\ Project/RTOS\ Playground/Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.c Common/Third_Party/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -I"D:/RTOS STM Project/Energy-dashboard-with-stm32-esp32/STM32_RTOS/016Task2_ADC_USART/AppFiles" -I"D:/RTOS STM Project/Energy-dashboard-with-stm32-esp32/STM32_RTOS/016Task2_ADC_USART/AppFiles/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.o: D:/RTOS\ STM\ Project/RTOS\ Playground/Common/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.c Common/Third_Party/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -I"D:/RTOS STM Project/Energy-dashboard-with-stm32-esp32/STM32_RTOS/016Task2_ADC_USART/AppFiles" -I"D:/RTOS STM Project/Energy-dashboard-with-stm32-esp32/STM32_RTOS/016Task2_ADC_USART/AppFiles/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Common/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Common-2f-Third_Party-2f-SEGGER-2f-SEGGER

clean-Common-2f-Third_Party-2f-SEGGER-2f-SEGGER:
	-$(RM) ./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT.cyclo ./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT.d ./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT.o ./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT.su ./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d ./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o ./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.cyclo ./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.d ./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.o ./Common/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.su ./Common/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.cyclo ./Common/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.d ./Common/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.o ./Common/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.su

.PHONY: clean-Common-2f-Third_Party-2f-SEGGER-2f-SEGGER

