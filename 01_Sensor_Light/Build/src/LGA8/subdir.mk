#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/LGA8/lga8.c

OBJS += \
./src/LGA8/lga8.o

C_DEPS += \
./src/LGA8/lga8.d

# Each subdirectory must supply rules for building sources it contributes
src/LGA8/%.o: ../src/LGA8/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	E:/LoongIDE/mips-2015.05/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
