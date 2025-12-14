################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/croutine.c \
C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/event_groups.c \
C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/list.c \
C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/queue.c \
C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/stream_buffer.c \
C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/tasks.c \
C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/timers.c 

OBJS += \
./Common/Third_Party/FreeRTOS/croutine.o \
./Common/Third_Party/FreeRTOS/event_groups.o \
./Common/Third_Party/FreeRTOS/list.o \
./Common/Third_Party/FreeRTOS/queue.o \
./Common/Third_Party/FreeRTOS/stream_buffer.o \
./Common/Third_Party/FreeRTOS/tasks.o \
./Common/Third_Party/FreeRTOS/timers.o 

C_DEPS += \
./Common/Third_Party/FreeRTOS/croutine.d \
./Common/Third_Party/FreeRTOS/event_groups.d \
./Common/Third_Party/FreeRTOS/list.d \
./Common/Third_Party/FreeRTOS/queue.d \
./Common/Third_Party/FreeRTOS/stream_buffer.d \
./Common/Third_Party/FreeRTOS/tasks.d \
./Common/Third_Party/FreeRTOS/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Common/Third_Party/FreeRTOS/croutine.o: C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/croutine.c Common/Third_Party/FreeRTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Third_Party/FreeRTOS/event_groups.o: C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/event_groups.c Common/Third_Party/FreeRTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Third_Party/FreeRTOS/list.o: C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/list.c Common/Third_Party/FreeRTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Third_Party/FreeRTOS/queue.o: C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/queue.c Common/Third_Party/FreeRTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Third_Party/FreeRTOS/stream_buffer.o: C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/stream_buffer.c Common/Third_Party/FreeRTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Third_Party/FreeRTOS/tasks.o: C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/tasks.c Common/Third_Party/FreeRTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Third_Party/FreeRTOS/timers.o: C:/ST/STM32CubeIDE_1.19.0/STM32CubeIDE/file/D/RTOS%20STM%20Project/RTOS\ Workspace/Common/Third_Party/FreeRTOS/timers.c Common/Third_Party/FreeRTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/include" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/Config" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/OS" -I"D:/RTOS STM Project/RTOS Workspace/Common/Third_Party/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Common-2f-Third_Party-2f-FreeRTOS

clean-Common-2f-Third_Party-2f-FreeRTOS:
	-$(RM) ./Common/Third_Party/FreeRTOS/croutine.cyclo ./Common/Third_Party/FreeRTOS/croutine.d ./Common/Third_Party/FreeRTOS/croutine.o ./Common/Third_Party/FreeRTOS/croutine.su ./Common/Third_Party/FreeRTOS/event_groups.cyclo ./Common/Third_Party/FreeRTOS/event_groups.d ./Common/Third_Party/FreeRTOS/event_groups.o ./Common/Third_Party/FreeRTOS/event_groups.su ./Common/Third_Party/FreeRTOS/list.cyclo ./Common/Third_Party/FreeRTOS/list.d ./Common/Third_Party/FreeRTOS/list.o ./Common/Third_Party/FreeRTOS/list.su ./Common/Third_Party/FreeRTOS/queue.cyclo ./Common/Third_Party/FreeRTOS/queue.d ./Common/Third_Party/FreeRTOS/queue.o ./Common/Third_Party/FreeRTOS/queue.su ./Common/Third_Party/FreeRTOS/stream_buffer.cyclo ./Common/Third_Party/FreeRTOS/stream_buffer.d ./Common/Third_Party/FreeRTOS/stream_buffer.o ./Common/Third_Party/FreeRTOS/stream_buffer.su ./Common/Third_Party/FreeRTOS/tasks.cyclo ./Common/Third_Party/FreeRTOS/tasks.d ./Common/Third_Party/FreeRTOS/tasks.o ./Common/Third_Party/FreeRTOS/tasks.su ./Common/Third_Party/FreeRTOS/timers.cyclo ./Common/Third_Party/FreeRTOS/timers.d ./Common/Third_Party/FreeRTOS/timers.o ./Common/Third_Party/FreeRTOS/timers.su

.PHONY: clean-Common-2f-Third_Party-2f-FreeRTOS

