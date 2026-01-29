################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Device/startup_stm32f411xe.s 

OBJS += \
./Device/startup_stm32f411xe.o 

S_DEPS += \
./Device/startup_stm32f411xe.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o: ../Device/%.s Device/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Device

clean-Device:
	-$(RM) ./Device/startup_stm32f411xe.d ./Device/startup_stm32f411xe.o

.PHONY: clean-Device

