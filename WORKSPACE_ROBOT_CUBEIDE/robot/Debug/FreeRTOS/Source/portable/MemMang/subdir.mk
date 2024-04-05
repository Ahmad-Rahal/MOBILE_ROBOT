################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/Source/portable/MemMang/heap_1.c \
../FreeRTOS/Source/portable/MemMang/heap_2.c \
../FreeRTOS/Source/portable/MemMang/heap_3.c \
../FreeRTOS/Source/portable/MemMang/heap_4.c \
../FreeRTOS/Source/portable/MemMang/heap_5.c 

OBJS += \
./FreeRTOS/Source/portable/MemMang/heap_1.o \
./FreeRTOS/Source/portable/MemMang/heap_2.o \
./FreeRTOS/Source/portable/MemMang/heap_3.o \
./FreeRTOS/Source/portable/MemMang/heap_4.o \
./FreeRTOS/Source/portable/MemMang/heap_5.o 

C_DEPS += \
./FreeRTOS/Source/portable/MemMang/heap_1.d \
./FreeRTOS/Source/portable/MemMang/heap_2.d \
./FreeRTOS/Source/portable/MemMang/heap_3.d \
./FreeRTOS/Source/portable/MemMang/heap_4.d \
./FreeRTOS/Source/portable/MemMang/heap_5.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/MemMang/%.o: ../FreeRTOS/Source/portable/MemMang/%.c FreeRTOS/Source/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DSTM32F411RETx -DNUCLEO_F411RE -DDEBUG -c -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeRTOS-2f-Source-2f-portable-2f-MemMang

clean-FreeRTOS-2f-Source-2f-portable-2f-MemMang:
	-$(RM) ./FreeRTOS/Source/portable/MemMang/heap_1.d ./FreeRTOS/Source/portable/MemMang/heap_1.o ./FreeRTOS/Source/portable/MemMang/heap_2.d ./FreeRTOS/Source/portable/MemMang/heap_2.o ./FreeRTOS/Source/portable/MemMang/heap_3.d ./FreeRTOS/Source/portable/MemMang/heap_3.o ./FreeRTOS/Source/portable/MemMang/heap_4.d ./FreeRTOS/Source/portable/MemMang/heap_4.o ./FreeRTOS/Source/portable/MemMang/heap_5.d ./FreeRTOS/Source/portable/MemMang/heap_5.o

.PHONY: clean-FreeRTOS-2f-Source-2f-portable-2f-MemMang

