################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.o: C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.c Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Common-2f-Third_Party-2f-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM4F

clean-Common-2f-Third_Party-2f-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM4F:
	-$(RM) ./Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.cyclo ./Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.d ./Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.o ./Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F/port.su

.PHONY: clean-Common-2f-Third_Party-2f-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM4F

