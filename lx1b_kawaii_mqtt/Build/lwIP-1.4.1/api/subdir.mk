#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../lwIP-1.4.1/api/api_lib.c \
../lwIP-1.4.1/api/api_msg.c \
../lwIP-1.4.1/api/err.c \
../lwIP-1.4.1/api/netbuf.c \
../lwIP-1.4.1/api/netdb.c \
../lwIP-1.4.1/api/netifapi.c \
../lwIP-1.4.1/api/sockets.c \
../lwIP-1.4.1/api/tcpip.c

OBJS += \
./lwIP-1.4.1/api/api_lib.o \
./lwIP-1.4.1/api/api_msg.o \
./lwIP-1.4.1/api/err.o \
./lwIP-1.4.1/api/netbuf.o \
./lwIP-1.4.1/api/netdb.o \
./lwIP-1.4.1/api/netifapi.o \
./lwIP-1.4.1/api/sockets.o \
./lwIP-1.4.1/api/tcpip.o

C_DEPS += \
./lwIP-1.4.1/api/api_lib.d \
./lwIP-1.4.1/api/api_msg.d \
./lwIP-1.4.1/api/err.d \
./lwIP-1.4.1/api/netbuf.d \
./lwIP-1.4.1/api/netdb.d \
./lwIP-1.4.1/api/netifapi.d \
./lwIP-1.4.1/api/sockets.d \
./lwIP-1.4.1/api/tcpip.d

# Each subdirectory must supply rules for building sources it contributes
lwIP-1.4.1/api/%.o: ../lwIP-1.4.1/api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -I"../kawaii_mqtt/network/mbedtls/include" -I"../kawaii_mqtt/common/log" -I"../kawaii_mqtt/mqttclient" -I"../kawaii_mqtt/platform/RT-Thread/" -I"../kawaii_mqtt/mqtt" -I"../kawaii_mqtt/common/" -I"../kawaii_mqtt/network" -I"../lwIP-1.4.1/include/posix" -I"../kawaii_mqtt/network/mbedtls/include/mbedtls" -I"../kawaii_mqtt/network/mbedtls/wrapper" -I"../RTT4/components/net/netdev/include" -I"../RTT4/components/net/sal/include/socket" -I"../RTT4/components/net/sal/include/" -I"../RTT4/components/net/sal/include/dfs_net" -I"../RTT4/components/net/at/include" -I"../RTT4/components/net/at/at_socket" -I"../RTT4/components/drivers/include/ipc/" -I"../RTT4/components/libc/compilers/newlib/" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

