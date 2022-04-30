#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/dhcp6.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/ethip6.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/icmp6.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/inet6.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/ip6.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/ip6_addr.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/ip6_frag.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/mld6.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/nd6.c

OBJS += \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/dhcp6.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/ethip6.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/icmp6.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/inet6.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/ip6.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/ip6_addr.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/ip6_frag.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/mld6.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/nd6.o

C_DEPS += \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/dhcp6.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/ethip6.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/icmp6.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/inet6.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/ip6.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/ip6_addr.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/ip6_frag.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/mld6.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/nd6.d

# Each subdirectory must supply rules for building sources it contributes
RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/%.o: ../RTT4/components/net/lwip/lwip-2.1.2/src/core/ipv6/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe  -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

