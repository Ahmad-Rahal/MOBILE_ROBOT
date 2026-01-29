################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/Source/portable/GCC/ARM_CM3_MPU/port.c 

OBJS += \
./FreeRTOS/Source/portable/GCC/ARM_CM3_MPU/port.o 

C_DEPS += \
./FreeRTOS/Source/portable/GCC/ARM_CM3_MPU/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/GCC/ARM_CM3_MPU/%.o: ../FreeRTOS/Source/portable/GCC/ARM_CM3_MPU/%.c FreeRTOS/Source/portable/GCC/ARM_CM3_MPU/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DSTM32F411RETx -DNUCLEO_F411RE -DDEBUG -c -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM3_MPU

clean-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM3_MPU:
	-$(RM) ./FreeRTOS/Source/portable/GCC/ARM_CM3_MPU/port.d ./FreeRTOS/Source/portable/GCC/ARM_CM3_MPU/port.o

.PHONY: clean-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM3_MPU

