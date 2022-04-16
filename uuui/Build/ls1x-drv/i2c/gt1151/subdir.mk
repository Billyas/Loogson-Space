#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../ls1x-drv/i2c/gt1151/gt1151.c \
../ls1x-drv/i2c/gt1151/touch.c

OBJS += \
./ls1x-drv/i2c/gt1151/gt1151.o \
./ls1x-drv/i2c/gt1151/touch.o

C_DEPS += \
./ls1x-drv/i2c/gt1151/gt1151.d \
./ls1x-drv/i2c/gt1151/touch.d

# Each subdirectory must supply rules for building sources it contributes
ls1x-drv/i2c/gt1151/%.o: ../ls1x-drv/i2c/gt1151/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RTEMS C Compiler'
	D:/LoongIDE/rtems-4.10/bin/mips-rtems4.10-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe  -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

