################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SystemClock.c \
../src/captDistIR.c \
../src/captDistUltrason.c \
../src/groveLCD.c \
../src/main.c \
../src/motorCommand.c \
../src/newlib_stubs.c \
../src/pixyCam2.c \
../src/quadEncoder.c \
../src/servoCommand.c \
../src/stm32f4xx_hal_msp.c \
../src/stm32f4xx_it.c \
../src/system_stm32f4xx.c \
../src/tickTimer.c \
../src/util.c 

OBJS += \
./src/SystemClock.o \
./src/captDistIR.o \
./src/captDistUltrason.o \
./src/groveLCD.o \
./src/main.o \
./src/motorCommand.o \
./src/newlib_stubs.o \
./src/pixyCam2.o \
./src/quadEncoder.o \
./src/servoCommand.o \
./src/stm32f4xx_hal_msp.o \
./src/stm32f4xx_it.o \
./src/system_stm32f4xx.o \
./src/tickTimer.o \
./src/util.o 

C_DEPS += \
./src/SystemClock.d \
./src/captDistIR.d \
./src/captDistUltrason.d \
./src/groveLCD.d \
./src/main.d \
./src/motorCommand.d \
./src/newlib_stubs.d \
./src/pixyCam2.d \
./src/quadEncoder.d \
./src/servoCommand.d \
./src/stm32f4xx_hal_msp.d \
./src/stm32f4xx_it.d \
./src/system_stm32f4xx.d \
./src/tickTimer.d \
./src/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DSTM32F411RETx -DNUCLEO_F411RE -DDEBUG -c -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src

clean-src:
	-$(RM) ./src/SystemClock.d ./src/SystemClock.o ./src/captDistIR.d ./src/captDistIR.o ./src/captDistUltrason.d ./src/captDistUltrason.o ./src/groveLCD.d ./src/groveLCD.o ./src/main.d ./src/main.o ./src/motorCommand.d ./src/motorCommand.o ./src/newlib_stubs.d ./src/newlib_stubs.o ./src/pixyCam2.d ./src/pixyCam2.o ./src/quadEncoder.d ./src/quadEncoder.o ./src/servoCommand.d ./src/servoCommand.o ./src/stm32f4xx_hal_msp.d ./src/stm32f4xx_hal_msp.o ./src/stm32f4xx_it.d ./src/stm32f4xx_it.o ./src/system_stm32f4xx.d ./src/system_stm32f4xx.o ./src/tickTimer.d ./src/tickTimer.o ./src/util.d ./src/util.o

.PHONY: clean-src

