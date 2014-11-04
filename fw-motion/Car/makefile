################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include src/mcu-files/subdir.mk
-include src/freertos/subdir.mk
-include src/freertos/portable/Softune/MB96340/subdir.mk
-include src/freertos/portable/MemMang/subdir.mk
-include src/api/wireless/subdir.mk
-include src/api/wireless/bluetooth/subdir.mk
-include src/api/wireless/AMB8420/subdir.mk
-include src/api/subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
OBJCOPY_FLASH += \
FW-Motion.mhx \

FLASHLYDUMMY += \
flashlydummy \


# All Target
all: FW-Motion.abs secondary-outputs

# Tool invocations
FW-Motion.abs: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Link object files'
	flnk907s -cpu MB96F348HSB -g -w 2 -Xset_rora -pl 60 -pw 132 -a -AL 2 -ro _INROM01=0x00fe0000/0x00ffffff -ra _INRAM01=0x00002240/0x00007fff -sc CONST/Data/BYTE=0xFF8000 -rg 0 -m FW-Motion.mp1 -mmi -slf FW-Motion.mps -NCI0302LIB -o "FW-Motion.abs" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

FW-Motion.mhx: FW-Motion.abs
	@echo 'Create Flash image (mhx format)'
	f2ms -S2 FW-Motion.abs -o FW-Motion.mhx -o "FW-Motion.mhx"
	@echo 'Finished building: $@'
	@echo ' '

flashlydummy: FW-Motion.mhx
	@echo 'Upload programm to device'
	flashly -cpu MB96F348HSB -c:0 -d:30 -r:DTR+ -Q:4 -E:FE0000 -E:FF0000 -E:DF0000 -nolog -newlog -msgok -P:FW-Motion.mhx
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(FLASHLYDUMMY)$(OBJCOPY_FLASH)$(EXECUTABLES) FW-Motion.abs
	-@echo ' '

secondary-outputs: $(OBJCOPY_FLASH) $(FLASHLYDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
