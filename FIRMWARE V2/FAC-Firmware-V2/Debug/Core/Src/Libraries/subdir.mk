################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Libraries/DMApwm.c \
../Core/Src/Libraries/LSM6DS3.c 

OBJS += \
./Core/Src/Libraries/DMApwm.o \
./Core/Src/Libraries/LSM6DS3.o 

C_DEPS += \
./Core/Src/Libraries/DMApwm.d \
./Core/Src/Libraries/LSM6DS3.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Libraries/%.o Core/Src/Libraries/%.su Core/Src/Libraries/%.cyclo: ../Core/Src/Libraries/%.c Core/Src/Libraries/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Libraries

clean-Core-2f-Src-2f-Libraries:
	-$(RM) ./Core/Src/Libraries/DMApwm.cyclo ./Core/Src/Libraries/DMApwm.d ./Core/Src/Libraries/DMApwm.o ./Core/Src/Libraries/DMApwm.su ./Core/Src/Libraries/LSM6DS3.cyclo ./Core/Src/Libraries/LSM6DS3.d ./Core/Src/Libraries/LSM6DS3.o ./Core/Src/Libraries/LSM6DS3.su

.PHONY: clean-Core-2f-Src-2f-Libraries

