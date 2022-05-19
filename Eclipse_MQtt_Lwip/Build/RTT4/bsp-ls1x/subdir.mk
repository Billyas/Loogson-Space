#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../RTT4/bsp-ls1x/drv_can.c \
../RTT4/bsp-ls1x/drv_fb.c \
../RTT4/bsp-ls1x/drv_nand.c \
../RTT4/bsp-ls1x/drv_pwm.c \
../RTT4/bsp-ls1x/drv_uart.c \
../RTT4/bsp-ls1x/rt_ls1x_drv_init.c

OBJS += \
./RTT4/bsp-ls1x/drv_can.o \
./RTT4/bsp-ls1x/drv_fb.o \
./RTT4/bsp-ls1x/drv_nand.o \
./RTT4/bsp-ls1x/drv_pwm.o \
./RTT4/bsp-ls1x/drv_uart.o \
./RTT4/bsp-ls1x/rt_ls1x_drv_init.o

C_DEPS += \
./RTT4/bsp-ls1x/drv_can.d \
./RTT4/bsp-ls1x/drv_fb.d \
./RTT4/bsp-ls1x/drv_nand.d \
./RTT4/bsp-ls1x/drv_pwm.d \
./RTT4/bsp-ls1x/drv_uart.d \
./RTT4/bsp-ls1x/rt_ls1x_drv_init.d

# Each subdirectory must supply rules for building sources it contributes
RTT4/bsp-ls1x/%.o: ../RTT4/bsp-ls1x/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

