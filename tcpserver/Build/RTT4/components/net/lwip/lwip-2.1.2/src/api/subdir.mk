#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../RTT4/components/net/lwip/lwip-2.1.2/src/api/api_lib.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/api/api_msg.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/api/err.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/api/if_api.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/api/netbuf.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/api/netdb.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/api/netifapi.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/api/sockets.c \
../RTT4/components/net/lwip/lwip-2.1.2/src/api/tcpip.c

OBJS += \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/api_lib.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/api_msg.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/err.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/if_api.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/netbuf.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/netdb.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/netifapi.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/sockets.o \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/tcpip.o

C_DEPS += \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/api_lib.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/api_msg.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/err.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/if_api.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/netbuf.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/netdb.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/netifapi.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/sockets.d \
./RTT4/components/net/lwip/lwip-2.1.2/src/api/tcpip.d

# Each subdirectory must supply rules for building sources it contributes
RTT4/components/net/lwip/lwip-2.1.2/src/api/%.o: ../RTT4/components/net/lwip/lwip-2.1.2/src/api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe  -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

