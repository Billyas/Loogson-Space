#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../ls1x-drv/rtc/ls1x_rtc.c

OBJS += \
./ls1x-drv/rtc/ls1x_rtc.o

C_DEPS += \
./ls1x-drv/rtc/ls1x_rtc.d

# Each subdirectory must supply rules for building sources it contributes
ls1x-drv/rtc/%.o: ../ls1x-drv/rtc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2015.05/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

