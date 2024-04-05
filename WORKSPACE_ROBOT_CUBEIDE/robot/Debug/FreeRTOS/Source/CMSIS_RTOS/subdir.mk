################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c 

OBJS += \
./FreeRTOS/Source/CMSIS_RTOS/cmsis_os.o 

C_DEPS += \
./FreeRTOS/Source/CMSIS_RTOS/cmsis_os.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/CMSIS_RTOS/%.o: ../FreeRTOS/Source/CMSIS_RTOS/%.c FreeRTOS/Source/CMSIS_RTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DSTM32F411RETx -DNUCLEO_F411RE -DDEBUG -c -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeRTOS-2f-Source-2f-CMSIS_RTOS

clean-FreeRTOS-2f-Source-2f-CMSIS_RTOS:
	-$(RM) ./FreeRTOS/Source/CMSIS_RTOS/cmsis_os.d ./FreeRTOS/Source/CMSIS_RTOS/cmsis_os.o

.PHONY: clean-FreeRTOS-2f-Source-2f-CMSIS_RTOS

