################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/freertos/portable/Softune/MB96340/__STD_LIB_sbrk.c \
../src/freertos/portable/Softune/MB96340/port.c 

OBJS += \
./src/freertos/portable/Softune/MB96340/__STD_LIB_sbrk.o \
./src/freertos/portable/Softune/MB96340/port.o 


# Each subdirectory must supply rules for building sources it contributes
src/freertos/portable/Softune/MB96340/%.o: ../src/freertos/portable/Softune/MB96340/%.c
	@echo 'Building file: $<'
	@echo 'Compiling C source files'
	fcc907s -cpu MB96F348HSB -g -w 5 -INF STACK -INF srcin -T p,-B -x vTaskSwitchContext,vTaskIncrementTick -xauto 30 -c -D __96340 -D RF_DEBUG -D WIRELESS_DEBUG=0 -D __CONFIG__=3 -D DEBUG=1 -I "C:\Users\Felix\git\unknownartist\fw-motion\src\mcu-files" -I "C:\Users\Felix\git\unknownartist\fw-motion\src\freertos\include" -I "C:\Users\Felix\git\unknownartist\fw-motion\src\freertos\portable\Softune\MB96340" -I "C:\Program Files (x86)\Eclipse-ES-ProSem/softune/lib/907/include" -O 4 -K SPEED -K NOUNROLL -K NOLIB -K NOEOPT -K ADDSP -K NOALIAS -B -model MEDIUM -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


