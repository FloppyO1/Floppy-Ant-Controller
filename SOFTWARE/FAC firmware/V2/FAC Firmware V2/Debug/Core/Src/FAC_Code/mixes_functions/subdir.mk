################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/FAC_Code/mixes_functions/fac_mixes.c \
../Core/Src/FAC_Code/mixes_functions/fac_template_mix.c 

OBJS += \
./Core/Src/FAC_Code/mixes_functions/fac_mixes.o \
./Core/Src/FAC_Code/mixes_functions/fac_template_mix.o 

C_DEPS += \
./Core/Src/FAC_Code/mixes_functions/fac_mixes.d \
./Core/Src/FAC_Code/mixes_functions/fac_template_mix.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/FAC_Code/mixes_functions/%.o Core/Src/FAC_Code/mixes_functions/%.su Core/Src/FAC_Code/mixes_functions/%.cyclo: ../Core/Src/FAC_Code/mixes_functions/%.c Core/Src/FAC_Code/mixes_functions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-FAC_Code-2f-mixes_functions

clean-Core-2f-Src-2f-FAC_Code-2f-mixes_functions:
	-$(RM) ./Core/Src/FAC_Code/mixes_functions/fac_mixes.cyclo ./Core/Src/FAC_Code/mixes_functions/fac_mixes.d ./Core/Src/FAC_Code/mixes_functions/fac_mixes.o ./Core/Src/FAC_Code/mixes_functions/fac_mixes.su ./Core/Src/FAC_Code/mixes_functions/fac_template_mix.cyclo ./Core/Src/FAC_Code/mixes_functions/fac_template_mix.d ./Core/Src/FAC_Code/mixes_functions/fac_template_mix.o ./Core/Src/FAC_Code/mixes_functions/fac_template_mix.su

.PHONY: clean-Core-2f-Src-2f-FAC_Code-2f-mixes_functions

