#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../RTT4/src/clock.c \
../RTT4/src/components.c \
../RTT4/src/cpu.c \
../RTT4/src/device.c \
../RTT4/src/idle.c \
../RTT4/src/ipc.c \
../RTT4/src/irq.c \
../RTT4/src/kservice.c \
../RTT4/src/mem.c \
../RTT4/src/memheap.c \
../RTT4/src/mempool.c \
../RTT4/src/object.c \
../RTT4/src/scheduler.c \
../RTT4/src/signal.c \
../RTT4/src/slab.c \
../RTT4/src/thread.c \
../RTT4/src/timer.c

OBJS += \
./RTT4/src/clock.o \
./RTT4/src/components.o \
./RTT4/src/cpu.o \
./RTT4/src/device.o \
./RTT4/src/idle.o \
./RTT4/src/ipc.o \
./RTT4/src/irq.o \
./RTT4/src/kservice.o \
./RTT4/src/mem.o \
./RTT4/src/memheap.o \
./RTT4/src/mempool.o \
./RTT4/src/object.o \
./RTT4/src/scheduler.o \
./RTT4/src/signal.o \
./RTT4/src/slab.o \
./RTT4/src/thread.o \
./RTT4/src/timer.o

C_DEPS += \
./RTT4/src/clock.d \
./RTT4/src/components.d \
./RTT4/src/cpu.d \
./RTT4/src/device.d \
./RTT4/src/idle.d \
./RTT4/src/ipc.d \
./RTT4/src/irq.d \
./RTT4/src/kservice.d \
./RTT4/src/mem.d \
./RTT4/src/memheap.d \
./RTT4/src/mempool.d \
./RTT4/src/object.d \
./RTT4/src/scheduler.d \
./RTT4/src/signal.d \
./RTT4/src/slab.d \
./RTT4/src/thread.d \
./RTT4/src/timer.d

# Each subdirectory must supply rules for building sources it contributes
RTT4/src/%.o: ../RTT4/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

