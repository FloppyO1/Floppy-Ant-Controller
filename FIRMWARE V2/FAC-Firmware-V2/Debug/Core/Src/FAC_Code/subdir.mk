################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/FAC_Code/fac_adc.c \
../Core/Src/FAC_Code/fac_app.c \
../Core/Src/FAC_Code/fac_battery.c \
../Core/Src/FAC_Code/fac_eeprom.c \
../Core/Src/FAC_Code/fac_imu.c \
../Core/Src/FAC_Code/fac_mapper.c \
../Core/Src/FAC_Code/fac_motors.c \
../Core/Src/FAC_Code/fac_ppm_receiver.c \
../Core/Src/FAC_Code/fac_pwm_receiver.c \
../Core/Src/FAC_Code/fac_servo.c \
../Core/Src/FAC_Code/fac_settings.c \
../Core/Src/FAC_Code/fac_std_receiver.c 

OBJS += \
./Core/Src/FAC_Code/fac_adc.o \
./Core/Src/FAC_Code/fac_app.o \
./Core/Src/FAC_Code/fac_battery.o \
./Core/Src/FAC_Code/fac_eeprom.o \
./Core/Src/FAC_Code/fac_imu.o \
./Core/Src/FAC_Code/fac_mapper.o \
./Core/Src/FAC_Code/fac_motors.o \
./Core/Src/FAC_Code/fac_ppm_receiver.o \
./Core/Src/FAC_Code/fac_pwm_receiver.o \
./Core/Src/FAC_Code/fac_servo.o \
./Core/Src/FAC_Code/fac_settings.o \
./Core/Src/FAC_Code/fac_std_receiver.o 

C_DEPS += \
./Core/Src/FAC_Code/fac_adc.d \
./Core/Src/FAC_Code/fac_app.d \
./Core/Src/FAC_Code/fac_battery.d \
./Core/Src/FAC_Code/fac_eeprom.d \
./Core/Src/FAC_Code/fac_imu.d \
./Core/Src/FAC_Code/fac_mapper.d \
./Core/Src/FAC_Code/fac_motors.d \
./Core/Src/FAC_Code/fac_ppm_receiver.d \
./Core/Src/FAC_Code/fac_pwm_receiver.d \
./Core/Src/FAC_Code/fac_servo.d \
./Core/Src/FAC_Code/fac_settings.d \
./Core/Src/FAC_Code/fac_std_receiver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/FAC_Code/%.o Core/Src/FAC_Code/%.su Core/Src/FAC_Code/%.cyclo: ../Core/Src/FAC_Code/%.c Core/Src/FAC_Code/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-FAC_Code

clean-Core-2f-Src-2f-FAC_Code:
	-$(RM) ./Core/Src/FAC_Code/fac_adc.cyclo ./Core/Src/FAC_Code/fac_adc.d ./Core/Src/FAC_Code/fac_adc.o ./Core/Src/FAC_Code/fac_adc.su ./Core/Src/FAC_Code/fac_app.cyclo ./Core/Src/FAC_Code/fac_app.d ./Core/Src/FAC_Code/fac_app.o ./Core/Src/FAC_Code/fac_app.su ./Core/Src/FAC_Code/fac_battery.cyclo ./Core/Src/FAC_Code/fac_battery.d ./Core/Src/FAC_Code/fac_battery.o ./Core/Src/FAC_Code/fac_battery.su ./Core/Src/FAC_Code/fac_eeprom.cyclo ./Core/Src/FAC_Code/fac_eeprom.d ./Core/Src/FAC_Code/fac_eeprom.o ./Core/Src/FAC_Code/fac_eeprom.su ./Core/Src/FAC_Code/fac_imu.cyclo ./Core/Src/FAC_Code/fac_imu.d ./Core/Src/FAC_Code/fac_imu.o ./Core/Src/FAC_Code/fac_imu.su ./Core/Src/FAC_Code/fac_mapper.cyclo ./Core/Src/FAC_Code/fac_mapper.d ./Core/Src/FAC_Code/fac_mapper.o ./Core/Src/FAC_Code/fac_mapper.su ./Core/Src/FAC_Code/fac_motors.cyclo ./Core/Src/FAC_Code/fac_motors.d ./Core/Src/FAC_Code/fac_motors.o ./Core/Src/FAC_Code/fac_motors.su ./Core/Src/FAC_Code/fac_ppm_receiver.cyclo ./Core/Src/FAC_Code/fac_ppm_receiver.d ./Core/Src/FAC_Code/fac_ppm_receiver.o ./Core/Src/FAC_Code/fac_ppm_receiver.su ./Core/Src/FAC_Code/fac_pwm_receiver.cyclo ./Core/Src/FAC_Code/fac_pwm_receiver.d ./Core/Src/FAC_Code/fac_pwm_receiver.o ./Core/Src/FAC_Code/fac_pwm_receiver.su ./Core/Src/FAC_Code/fac_servo.cyclo ./Core/Src/FAC_Code/fac_servo.d ./Core/Src/FAC_Code/fac_servo.o ./Core/Src/FAC_Code/fac_servo.su ./Core/Src/FAC_Code/fac_settings.cyclo ./Core/Src/FAC_Code/fac_settings.d ./Core/Src/FAC_Code/fac_settings.o ./Core/Src/FAC_Code/fac_settings.su ./Core/Src/FAC_Code/fac_std_receiver.cyclo ./Core/Src/FAC_Code/fac_std_receiver.d ./Core/Src/FAC_Code/fac_std_receiver.o ./Core/Src/FAC_Code/fac_std_receiver.su

.PHONY: clean-Core-2f-Src-2f-FAC_Code

