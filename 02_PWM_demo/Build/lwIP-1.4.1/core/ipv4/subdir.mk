#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../lwIP-1.4.1/core/ipv4/autoip.c \
../lwIP-1.4.1/core/ipv4/icmp.c \
../lwIP-1.4.1/core/ipv4/igmp.c \
../lwIP-1.4.1/core/ipv4/inet.c \
../lwIP-1.4.1/core/ipv4/inet_chksum.c \
../lwIP-1.4.1/core/ipv4/ip.c \
../lwIP-1.4.1/core/ipv4/ip_addr.c \
../lwIP-1.4.1/core/ipv4/ip_frag.c

OBJS += \
./lwIP-1.4.1/core/ipv4/autoip.o \
./lwIP-1.4.1/core/ipv4/icmp.o \
./lwIP-1.4.1/core/ipv4/igmp.o \
./lwIP-1.4.1/core/ipv4/inet.o \
./lwIP-1.4.1/core/ipv4/inet_chksum.o \
./lwIP-1.4.1/core/ipv4/ip.o \
./lwIP-1.4.1/core/ipv4/ip_addr.o \
./lwIP-1.4.1/core/ipv4/ip_frag.o

C_DEPS += \
./lwIP-1.4.1/core/ipv4/autoip.d \
./lwIP-1.4.1/core/ipv4/icmp.d \
./lwIP-1.4.1/core/ipv4/igmp.d \
./lwIP-1.4.1/core/ipv4/inet.d \
./lwIP-1.4.1/core/ipv4/inet_chksum.d \
./lwIP-1.4.1/core/ipv4/ip.d \
./lwIP-1.4.1/core/ipv4/ip_addr.d \
./lwIP-1.4.1/core/ipv4/ip_frag.d

# Each subdirectory must supply rules for building sources it contributes
lwIP-1.4.1/core/ipv4/%.o: ../lwIP-1.4.1/core/ipv4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

