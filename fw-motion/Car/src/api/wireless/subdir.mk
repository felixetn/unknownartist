################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/api/wireless/wireless.c 

OBJS += \
./src/api/wireless/wireless.o 


# Each subdirectory must supply rules for building sources it contributes
src/api/wireless/%.o: ../src/api/wireless/%.c
	@echo 'Building file: $<'
	@echo 'Compiling C source files'
	fcc907s -cpu MB96F348HSB -g -w 5 -INF STACK -INF srcin -T p,-B -x vTaskSwitchContext,vTaskIncrementTick -xauto 30 -c -D __96340 -D RF_DEBUG -D WIRELESS_DEBUG=0 -D __CONFIG__=3 -D DEBUG=1 -I "C:\Users\Daniel\Documents\RefactoredUnknown\fw-motion\src\mcu-files" -I "C:\Users\Daniel\Documents\RefactoredUnknown\fw-motion\src\freertos\include" -I "C:\Users\Daniel\Documents\RefactoredUnknown\fw-motion\src\freertos\portable\Softune\MB96340" -I "C:\Program Files (x86)\Eclipse-ES-ProSem/softune/lib/907/include" -O 4 -K SPEED -K NOUNROLL -K NOLIB -K NOEOPT -K ADDSP -K NOALIAS -B -model MEDIUM -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


