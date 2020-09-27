################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App/appLcdSpeedTest.c \
../Core/Src/App/beer_60x100_16.c 

OBJS += \
./Core/Src/App/appLcdSpeedTest.o \
./Core/Src/App/beer_60x100_16.o 

C_DEPS += \
./Core/Src/App/appLcdSpeedTest.d \
./Core/Src/App/beer_60x100_16.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/App/appLcdSpeedTest.o: ../Core/Src/App/appLcdSpeedTest.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -c -I../Core/Inc -I"C:/Users/Calcutt/Documents/QUT MS/QUTMS_SteeringWheel/software/QUTMS_LCD/Core/Src" -I"C:/Users/Calcutt/Documents/QUT MS/QUTMS_SteeringWheel/software/QUTMS_LCD/Core/Src/Lcd" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/App/appLcdSpeedTest.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/App/beer_60x100_16.o: ../Core/Src/App/beer_60x100_16.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -c -I../Core/Inc -I"C:/Users/Calcutt/Documents/QUT MS/QUTMS_SteeringWheel/software/QUTMS_LCD/Core/Src" -I"C:/Users/Calcutt/Documents/QUT MS/QUTMS_SteeringWheel/software/QUTMS_LCD/Core/Src/Lcd" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/App/beer_60x100_16.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

