#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
ASM_SRCS += \
../RTT4/port/cache.S \
../RTT4/port/context.S \
../RTT4/port/irq_s.S \
../RTT4/port/mips_timer.S \
../RTT4/port/start.S \
../RTT4/port/tlb.S

C_SRCS += \
../RTT4/port/bsp_start.c \
../RTT4/port/exception.c \
../RTT4/port/inittlb.c \
../RTT4/port/irq.c \
../RTT4/port/stack.c \
../RTT4/port/tick.c

STARTO += ./RTT4/port/start.o

OBJS += \
./RTT4/port/bsp_start.o \
./RTT4/port/cache.o \
./RTT4/port/context.o \
./RTT4/port/exception.o \
./RTT4/port/inittlb.o \
./RTT4/port/irq.o \
./RTT4/port/irq_s.o \
./RTT4/port/mips_timer.o \
./RTT4/port/stack.o \
./RTT4/port/tick.o \
./RTT4/port/tlb.o

ASM_DEPS += \
./RTT4/port/cache.d \
./RTT4/port/context.d \
./RTT4/port/irq_s.d \
./RTT4/port/mips_timer.d \
./RTT4/port/start.d \
./RTT4/port/tlb.d

C_DEPS += \
./RTT4/port/bsp_start.d \
./RTT4/port/exception.d \
./RTT4/port/inittlb.d \
./RTT4/port/irq.d \
./RTT4/port/stack.d \
./RTT4/port/tick.d

# Each subdirectory must supply rules for building sources it contributes
RTT4/port/%.o: ../RTT4/port/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTT4/port/%.o: ../RTT4/port/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

