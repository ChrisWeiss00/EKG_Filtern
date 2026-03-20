################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Lib/startup_stm32l100rctx.s 

C_SRCS += \
../Lib/adc.c \
../Lib/dac.c \
../Lib/lcd.c \
../Lib/port.c \
../Lib/pwm.c \
../Lib/system_stm32l1xx.c \
../Lib/timer.c \
../Lib/uart.c 

S_DEPS += \
./Lib/startup_stm32l100rctx.d 

C_DEPS += \
./Lib/adc.d \
./Lib/dac.d \
./Lib/lcd.d \
./Lib/port.d \
./Lib/pwm.d \
./Lib/system_stm32l1xx.d \
./Lib/timer.d \
./Lib/uart.d 

OBJS += \
./Lib/adc.o \
./Lib/dac.o \
./Lib/lcd.o \
./Lib/port.o \
./Lib/pwm.o \
./Lib/startup_stm32l100rctx.o \
./Lib/system_stm32l1xx.o \
./Lib/timer.o \
./Lib/uart.o 


# Each subdirectory must supply rules for building sources it contributes
Lib/%.o Lib/%.su: ../Lib/%.c Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32L1 -DSTM32 -DSTM32L100RCTx -DDEBUG -c -I"C:/Users/Mein Pc/Downloads/template Smt32cubed - Kopie/Template/src" -I"C:/Users/Mein Pc/Downloads/template Smt32cubed - Kopie/Template/Lib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Lib/%.o: ../Lib/%.s Lib/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Lib

clean-Lib:
	-$(RM) ./Lib/adc.d ./Lib/adc.o ./Lib/adc.su ./Lib/dac.d ./Lib/dac.o ./Lib/dac.su ./Lib/lcd.d ./Lib/lcd.o ./Lib/lcd.su ./Lib/port.d ./Lib/port.o ./Lib/port.su ./Lib/pwm.d ./Lib/pwm.o ./Lib/pwm.su ./Lib/startup_stm32l100rctx.d ./Lib/startup_stm32l100rctx.o ./Lib/system_stm32l1xx.d ./Lib/system_stm32l1xx.o ./Lib/system_stm32l1xx.su ./Lib/timer.d ./Lib/timer.o ./Lib/timer.su ./Lib/uart.d ./Lib/uart.o ./Lib/uart.su

.PHONY: clean-Lib

