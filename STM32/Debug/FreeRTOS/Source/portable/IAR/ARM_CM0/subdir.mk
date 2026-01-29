################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../FreeRTOS/Source/portable/IAR/ARM_CM0/portasm.s 

C_SRCS += \
../FreeRTOS/Source/portable/IAR/ARM_CM0/port.c 

OBJS += \
./FreeRTOS/Source/portable/IAR/ARM_CM0/port.o \
./FreeRTOS/Source/portable/IAR/ARM_CM0/portasm.o 

S_DEPS += \
./FreeRTOS/Source/portable/IAR/ARM_CM0/portasm.d 

C_DEPS += \
./FreeRTOS/Source/portable/IAR/ARM_CM0/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/IAR/ARM_CM0/%.o: ../FreeRTOS/Source/portable/IAR/ARM_CM0/%.c FreeRTOS/Source/portable/IAR/ARM_CM0/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DSTM32F411RETx -DNUCLEO_F411RE -DDEBUG -c -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
FreeRTOS/Source/portable/IAR/ARM_CM0/%.o: ../FreeRTOS/Source/portable/IAR/ARM_CM0/%.s FreeRTOS/Source/portable/IAR/ARM_CM0/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-FreeRTOS-2f-Source-2f-portable-2f-IAR-2f-ARM_CM0

clean-FreeRTOS-2f-Source-2f-portable-2f-IAR-2f-ARM_CM0:
	-$(RM) ./FreeRTOS/Source/portable/IAR/ARM_CM0/port.d ./FreeRTOS/Source/portable/IAR/ARM_CM0/port.o ./FreeRTOS/Source/portable/IAR/ARM_CM0/portasm.d ./FreeRTOS/Source/portable/IAR/ARM_CM0/portasm.o

.PHONY: clean-FreeRTOS-2f-Source-2f-portable-2f-IAR-2f-ARM_CM0

