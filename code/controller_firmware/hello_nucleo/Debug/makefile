################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/custom/spi/subdir.mk
-include Drivers/custom/pwm/subdir.mk
-include Drivers/custom/pid_reg/subdir.mk
-include Drivers/custom/nvic/subdir.mk
-include Drivers/custom/gpio/subdir.mk
-include Drivers/custom/fsm/subdir.mk
-include Drivers/custom/exti/subdir.mk
-include Drivers/custom/drv/subdir.mk
-include Drivers/custom/adc/subdir.mk
-include Drivers/STM32F3xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
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
hello_nucleo.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
hello_nucleo.list \


# All Target
all: hello_nucleo.elf secondary-outputs

# Tool invocations
hello_nucleo.elf: $(OBJS) $(USER_OBJS) C:\Users\Justin\Desktop\eecs373\proj\controller_firmware\hello_nucleo\STM32F303RETX_FLASH.ld
	arm-none-eabi-gcc -o "hello_nucleo.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Justin\Desktop\eecs373\proj\controller_firmware\hello_nucleo\STM32F303RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="hello_nucleo.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

hello_nucleo.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "hello_nucleo.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
