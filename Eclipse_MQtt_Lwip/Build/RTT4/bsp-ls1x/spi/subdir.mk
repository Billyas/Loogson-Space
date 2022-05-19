#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../RTT4/bsp-ls1x/spi/drv_w25x40.c \
../RTT4/bsp-ls1x/spi/drv_xpt2046.c

OBJS += \
./RTT4/bsp-ls1x/spi/drv_w25x40.o \
./RTT4/bsp-ls1x/spi/drv_xpt2046.o

C_DEPS += \
./RTT4/bsp-ls1x/spi/drv_w25x40.d \
./RTT4/bsp-ls1x/spi/drv_xpt2046.d

# Each subdirectory must supply rules for building sources it contributes
RTT4/bsp-ls1x/spi/%.o: ../RTT4/bsp-ls1x/spi/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

