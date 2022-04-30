#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/altcp.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/altcp_alloc.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/altcp_tcp.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/def.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/dns.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/inet_chksum.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/init.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/ip.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/mem.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/memp.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/netif.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/pbuf.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/raw.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/stats.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/sys.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/tcp.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/tcp_in.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/tcp_out.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/timeouts.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/core/udp.c

OBJS += \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/altcp.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/altcp_alloc.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/altcp_tcp.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/def.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/dns.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/inet_chksum.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/init.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ip.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/mem.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/memp.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/netif.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/pbuf.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/raw.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/stats.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/sys.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/tcp.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/tcp_in.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/tcp_out.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/timeouts.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/udp.o

C_DEPS += \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/altcp.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/altcp_alloc.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/altcp_tcp.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/def.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/dns.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/inet_chksum.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/init.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/ip.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/mem.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/memp.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/netif.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/pbuf.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/raw.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/stats.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/sys.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/tcp.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/tcp_in.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/tcp_out.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/timeouts.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/core/udp.d

# Each subdirectory must supply rules for building sources it contributes
RTT4/components/net/lwip/lwip-2.1.2/src/core/%.o: ../RTT4/components/net/lwip/lwip-2.1.2/src/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe  -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

