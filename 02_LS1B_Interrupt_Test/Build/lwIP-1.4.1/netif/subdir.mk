#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../lwIP-1.4.1/netif/etharp.c \
../lwIP-1.4.1/netif/ethernetif.c \
../lwIP-1.4.1/netif/slipif.c

OBJS += \
./lwIP-1.4.1/netif/etharp.o \
./lwIP-1.4.1/netif/ethernetif.o \
./lwIP-1.4.1/netif/slipif.o

C_DEPS += \
./lwIP-1.4.1/netif/etharp.d \
./lwIP-1.4.1/netif/ethernetif.d \
./lwIP-1.4.1/netif/slipif.d

# Each subdirectory must supply rules for building sources it contributes
lwIP-1.4.1/netif/%.o: ../lwIP-1.4.1/netif/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

