################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/drv/drv_gpio.c \
../src/drv/drv_i2c.c \
../src/drv/drv_spi.c \
../src/drv/drv_uart.c 

OBJS += \
./src/drv/drv_gpio.o \
./src/drv/drv_i2c.o \
./src/drv/drv_spi.o \
./src/drv/drv_uart.o 

C_DEPS += \
./src/drv/drv_gpio.d \
./src/drv/drv_i2c.d \
./src/drv/drv_spi.d \
./src/drv/drv_uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/drv/%.o: ../src/drv/%.c src/drv/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DSTM32F411RETx -DNUCLEO_F411RE -DDEBUG -c -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-drv

clean-src-2f-drv:
	-$(RM) ./src/drv/drv_gpio.d ./src/drv/drv_gpio.o ./src/drv/drv_i2c.d ./src/drv/drv_i2c.o ./src/drv/drv_spi.d ./src/drv/drv_spi.o ./src/drv/drv_uart.d ./src/drv/drv_uart.o

.PHONY: clean-src-2f-drv

