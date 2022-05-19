#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../kawaii_mqtt/platform/RT-Thread/platform_memory.c \
../kawaii_mqtt/platform/RT-Thread/platform_mutex.c \
../kawaii_mqtt/platform/RT-Thread/platform_net_socket.c \
../kawaii_mqtt/platform/RT-Thread/platform_thread.c \
../kawaii_mqtt/platform/RT-Thread/platform_timer.c

OBJS += \
./kawaii_mqtt/platform/RT-Thread/platform_memory.o \
./kawaii_mqtt/platform/RT-Thread/platform_mutex.o \
./kawaii_mqtt/platform/RT-Thread/platform_net_socket.o \
./kawaii_mqtt/platform/RT-Thread/platform_thread.o \
./kawaii_mqtt/platform/RT-Thread/platform_timer.o

C_DEPS += \
./kawaii_mqtt/platform/RT-Thread/platform_memory.d \
./kawaii_mqtt/platform/RT-Thread/platform_mutex.d \
./kawaii_mqtt/platform/RT-Thread/platform_net_socket.d \
./kawaii_mqtt/platform/RT-Thread/platform_thread.d \
./kawaii_mqtt/platform/RT-Thread/platform_timer.d

# Each subdirectory must supply rules for building sources it contributes
kawaii_mqtt/platform/RT-Thread/%.o: ../kawaii_mqtt/platform/RT-Thread/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -I"../kawaii_mqtt/network/mbedtls/include" -I"../kawaii_mqtt/common/log" -I"../kawaii_mqtt/mqttclient" -I"../kawaii_mqtt/platform/RT-Thread/" -I"../kawaii_mqtt/mqtt" -I"../kawaii_mqtt/common/" -I"../kawaii_mqtt/network" -I"../lwIP-1.4.1/include/posix" -I"../kawaii_mqtt/network/mbedtls/include/mbedtls" -I"../kawaii_mqtt/network/mbedtls/wrapper" -I"../RTT4/components/net/netdev/include" -I"../RTT4/components/net/sal/include/socket" -I"../RTT4/components/net/sal/include/" -I"../RTT4/components/net/sal/include/dfs_net" -I"../RTT4/components/net/at/include" -I"../RTT4/components/net/at/at_socket" -I"../RTT4/components/drivers/include/ipc/" -I"../RTT4/components/libc/compilers/newlib/" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

