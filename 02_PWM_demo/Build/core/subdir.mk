#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
ASM_SRCS += \
../core/cache.S \
../core/irq_s.S \
../core/mips_timer.S \
../core/start.S \
../core/tlb.S

C_SRCS += \
../core/bsp_start.c \
../core/exception.c \
../core/inittlb.c \
../core/irq.c \
../core/tick.c

STARTO += ./core/start.o

OBJS += \
./core/bsp_start.o \
./core/cache.o \
./core/exception.o \
./core/inittlb.o \
./core/irq.o \
./core/irq_s.o \
./core/mips_timer.o \
./core/tick.o \
./core/tlb.o

ASM_DEPS += \
./core/cache.d \
./core/irq_s.d \
./core/mips_timer.d \
./core/start.d \
./core/tlb.d

C_DEPS += \
./core/bsp_start.d \
./core/exception.d \
./core/inittlb.d \
./core/irq.d \
./core/tick.d

# Each subdirectory must supply rules for building sources it contributes
core/%.o: ../core/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

core/%.o: ../core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

