################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../_Libraries/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.c 

OBJS += \
./_Libraries/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.o 

C_DEPS += \
./_Libraries/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
_Libraries/BSP/STM32F4xx-Nucleo/%.o: ../_Libraries/BSP/STM32F4xx-Nucleo/%.c _Libraries/BSP/STM32F4xx-Nucleo/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DSTM32F411RETx -DNUCLEO_F411RE -DDEBUG -c -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-_Libraries-2f-BSP-2f-STM32F4xx-2d-Nucleo

clean-_Libraries-2f-BSP-2f-STM32F4xx-2d-Nucleo:
	-$(RM) ./_Libraries/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.d ./_Libraries/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.o

.PHONY: clean-_Libraries-2f-BSP-2f-STM32F4xx-2d-Nucleo

