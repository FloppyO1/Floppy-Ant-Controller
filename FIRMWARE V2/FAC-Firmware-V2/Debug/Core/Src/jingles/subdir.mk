################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/jingles/fac_jingles.c 

OBJS += \
./Core/Src/jingles/fac_jingles.o 

C_DEPS += \
./Core/Src/jingles/fac_jingles.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/jingles/%.o Core/Src/jingles/%.su Core/Src/jingles/%.cyclo: ../Core/Src/jingles/%.c Core/Src/jingles/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-jingles

clean-Core-2f-Src-2f-jingles:
	-$(RM) ./Core/Src/jingles/fac_jingles.cyclo ./Core/Src/jingles/fac_jingles.d ./Core/Src/jingles/fac_jingles.o ./Core/Src/jingles/fac_jingles.su

.PHONY: clean-Core-2f-Src-2f-jingles

