#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../lwIP-1.4.1/core/def.c \
../lwIP-1.4.1/core/dhcp.c \
../lwIP-1.4.1/core/dns.c \
../lwIP-1.4.1/core/init.c \
../lwIP-1.4.1/core/mem.c \
../lwIP-1.4.1/core/memp.c \
../lwIP-1.4.1/core/netif.c \
../lwIP-1.4.1/core/pbuf.c \
../lwIP-1.4.1/core/raw.c \
../lwIP-1.4.1/core/stats.c \
../lwIP-1.4.1/core/sys.c \
../lwIP-1.4.1/core/tcp.c \
../lwIP-1.4.1/core/tcp_in.c \
../lwIP-1.4.1/core/tcp_out.c \
../lwIP-1.4.1/core/udp.c \
../lwIP-1.4.1/core/_timers.c

OBJS += \
./lwIP-1.4.1/core/def.o \
./lwIP-1.4.1/core/dhcp.o \
./lwIP-1.4.1/core/dns.o \
./lwIP-1.4.1/core/init.o \
./lwIP-1.4.1/core/mem.o \
./lwIP-1.4.1/core/memp.o \
./lwIP-1.4.1/core/netif.o \
./lwIP-1.4.1/core/pbuf.o \
./lwIP-1.4.1/core/raw.o \
./lwIP-1.4.1/core/stats.o \
./lwIP-1.4.1/core/sys.o \
./lwIP-1.4.1/core/tcp.o \
./lwIP-1.4.1/core/tcp_in.o \
./lwIP-1.4.1/core/tcp_out.o \
./lwIP-1.4.1/core/udp.o \
./lwIP-1.4.1/core/_timers.o

C_DEPS += \
./lwIP-1.4.1/core/def.d \
./lwIP-1.4.1/core/dhcp.d \
./lwIP-1.4.1/core/dns.d \
./lwIP-1.4.1/core/init.d \
./lwIP-1.4.1/core/mem.d \
./lwIP-1.4.1/core/memp.d \
./lwIP-1.4.1/core/netif.d \
./lwIP-1.4.1/core/pbuf.d \
./lwIP-1.4.1/core/raw.d \
./lwIP-1.4.1/core/stats.d \
./lwIP-1.4.1/core/sys.d \
./lwIP-1.4.1/core/tcp.d \
./lwIP-1.4.1/core/tcp_in.d \
./lwIP-1.4.1/core/tcp_out.d \
./lwIP-1.4.1/core/udp.d \
./lwIP-1.4.1/core/_timers.d

# Each subdirectory must supply rules for building sources it contributes
lwIP-1.4.1/core/%.o: ../lwIP-1.4.1/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

