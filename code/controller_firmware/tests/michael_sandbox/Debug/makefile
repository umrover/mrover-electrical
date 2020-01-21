################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/pwm/subdir.mk
-include Drivers/gpio/subdir.mk
-include Drivers/fsm/subdir.mk
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
michael_sandbox.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
michael_sandbox.list \


# All Target
all: michael_sandbox.elf secondary-outputs

# Tool invocations
michael_sandbox.elf: $(OBJS) $(USER_OBJS) C:\git\eecs373_final_project\controller_firmware\tests\michael_sandbox\STM32F303RETX_FLASH.ld
	arm-none-eabi-gcc -o "michael_sandbox.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\git\eecs373_final_project\controller_firmware\tests\michael_sandbox\STM32F303RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="michael_sandbox.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

michael_sandbox.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "michael_sandbox.list"
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
