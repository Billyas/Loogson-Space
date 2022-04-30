#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/autoip.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/icmp.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/igmp.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/inet.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/inet_chksum.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/ip.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/ip_addr.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/ip_frag.c

OBJS += \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/autoip.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/icmp.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/igmp.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/inet.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/inet_chksum.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/ip.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/ip_addr.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/ip_frag.o

C_DEPS += \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/autoip.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/icmp.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/igmp.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/inet.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/inet_chksum.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/ip.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/ip_addr.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/ip_frag.d

# Each subdirectory must supply rules for building sources it contributes
RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/%.o: ../RTT4/components/net/lwip/lwip-1.4.1/src/core/ipv4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe  -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

