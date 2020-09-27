################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/Lcd/Fonts/subdir.mk
-include Core/Src/Lcd/subdir.mk
-include Core/Src/App/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
QUTMS_LCD.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
QUTMS_LCD.list \

OBJCOPY_BIN += \
QUTMS_LCD.bin \


# All Target
all: QUTMS_LCD.elf secondary-outputs

# Tool invocations
QUTMS_LCD.elf: $(OBJS) $(USER_OBJS) C:\Users\Calcutt\Documents\QUT\ MS\QUTMS_SteeringWheel\software\QUTMS_LCD\STM32F446RETX_FLASH.ld
	arm-none-eabi-gcc -o "QUTMS_LCD.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Calcutt\Documents\QUT MS\QUTMS_SteeringWheel\software\QUTMS_LCD\STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="QUTMS_LCD.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

QUTMS_LCD.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "QUTMS_LCD.list"
	@echo 'Finished building: $@'
	@echo ' '

QUTMS_LCD.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "QUTMS_LCD.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
