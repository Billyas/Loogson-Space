#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/auth.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/chap.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/chpms.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/fsm.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/ipcp.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/lcp.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/magic.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/md5.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/pap.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/ppp.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/ppp_oe.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/randm.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/vj.c

OBJS += \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/auth.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/chap.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/chpms.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/fsm.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/ipcp.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/lcp.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/magic.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/md5.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/pap.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/ppp.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/ppp_oe.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/randm.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/vj.o

C_DEPS += \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/auth.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/chap.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/chpms.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/fsm.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/ipcp.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/lcp.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/magic.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/md5.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/pap.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/ppp.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/ppp_oe.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/randm.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/vj.d

# Each subdirectory must supply rules for building sources it contributes
RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/%.o: ../RTT4/components/net/lwip/lwip-1.4.1/src/netif/ppp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe  -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

