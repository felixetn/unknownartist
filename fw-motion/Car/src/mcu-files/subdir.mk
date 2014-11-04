################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/mcu-files/vectors.c 

ASM_SRCS += \
../src/mcu-files/mb96348hs.asm \
../src/mcu-files/start.asm 

OBJS += \
./src/mcu-files/mb96348hs.o \
./src/mcu-files/start.o \
./src/mcu-files/vectors.o 


# Each subdirectory must supply rules for building sources it contributes
src/mcu-files/%.o: ../src/mcu-files/%.asm
	@echo 'Building file: $<'
	@echo 'Assembling source files'
	fasm907s -cpu MB96F348HSB -g -w 2 -pl 60 -pw 100 -lexp OBJ -D __CONFIG__=3 -linf OFF -lsrc OFF -lsec OFF -lcros OFF -linc OFF -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/mcu-files/%.o: ../src/mcu-files/%.c
	@echo 'Building file: $<'
	@echo 'Compiling C source files'
	fcc907s -cpu MB96F348HSB -g -w 5 -INF STACK -INF srcin -T p,-B -x vTaskSwitchContext,vTaskIncrementTick -xauto 30 -c -D __96340 -D RF_DEBUG -D WIRELESS_DEBUG=0 -D __CONFIG__=3 -D DEBUG=1 -I "C:\Users\Felix\git\unknownartist\fw-motion\src\mcu-files" -I "C:\Users\Felix\git\unknownartist\fw-motion\src\freertos\include" -I "C:\Users\Felix\git\unknownartist\fw-motion\src\freertos\portable\Softune\MB96340" -I "C:\Program Files (x86)\Eclipse-ES-ProSem/softune/lib/907/include" -O 4 -K SPEED -K NOUNROLL -K NOLIB -K NOEOPT -K ADDSP -K NOALIAS -B -model MEDIUM -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


