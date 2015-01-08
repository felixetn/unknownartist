################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/api/adc.c \
../src/api/api.c \
../src/api/ausweichen.c \
../src/api/battery.c \
../src/api/carid.c \
../src/api/drive.c \
../src/api/fram.c \
../src/api/gablung.c \
../src/api/i2c.c \
../src/api/interrupts.c \
../src/api/linesensor.c \
../src/api/navigation.c \
../src/api/parken.c \
../src/api/printf_stdarg.c \
../src/api/regler.c \
../src/api/runtime_stats.c \
../src/api/seg.c \
../src/api/serial.c \
../src/api/transitionen.c \
../src/api/trigonometry.c \
../src/api/us.c \
../src/api/us_i2c.c \
../src/api/util.c 

OBJS += \
./src/api/adc.o \
./src/api/api.o \
./src/api/ausweichen.o \
./src/api/battery.o \
./src/api/carid.o \
./src/api/drive.o \
./src/api/fram.o \
./src/api/gablung.o \
./src/api/i2c.o \
./src/api/interrupts.o \
./src/api/linesensor.o \
./src/api/navigation.o \
./src/api/parken.o \
./src/api/printf_stdarg.o \
./src/api/regler.o \
./src/api/runtime_stats.o \
./src/api/seg.o \
./src/api/serial.o \
./src/api/transitionen.o \
./src/api/trigonometry.o \
./src/api/us.o \
./src/api/us_i2c.o \
./src/api/util.o 


# Each subdirectory must supply rules for building sources it contributes
src/api/%.o: ../src/api/%.c
	@echo 'Building file: $<'
	@echo 'Compiling C source files'
	fcc907s -cpu MB96F348HSB -g -w 5 -INF STACK -INF srcin -T p,-B -x vTaskSwitchContext,vTaskIncrementTick -xauto 30 -c -D __96340 -D RF_DEBUG -D WIRELESS_DEBUG=0 -D __CONFIG__=3 -D DEBUG=1 -I "C:\Users\Daniel\Documents\RefactoredUnknown\fw-motion\src\mcu-files" -I "C:\Users\Daniel\Documents\RefactoredUnknown\fw-motion\src\freertos\include" -I "C:\Users\Daniel\Documents\RefactoredUnknown\fw-motion\src\freertos\portable\Softune\MB96340" -I "C:\Program Files (x86)\Eclipse-ES-ProSem/softune/lib/907/include" -O 4 -K SPEED -K NOUNROLL -K NOLIB -K NOEOPT -K ADDSP -K NOALIAS -B -model MEDIUM -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


