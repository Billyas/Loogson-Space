#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../mqtt/MQTTConnectClient.c \
../mqtt/MQTTConnectServer.c \
../mqtt/MQTTDeserializePublish.c \
../mqtt/MQTTFormat.c \
../mqtt/MQTTPacket.c \
../mqtt/MQTTSerializePublish.c \
../mqtt/MQTTSubscribeClient.c \
../mqtt/MQTTSubscribeServer.c \
../mqtt/MQTTUnsubscribeClient.c \
../mqtt/MQTTUnsubscribeServer.c \
../mqtt/transport.c

OBJS += \
./mqtt/MQTTConnectClient.o \
./mqtt/MQTTConnectServer.o \
./mqtt/MQTTDeserializePublish.o \
./mqtt/MQTTFormat.o \
./mqtt/MQTTPacket.o \
./mqtt/MQTTSerializePublish.o \
./mqtt/MQTTSubscribeClient.o \
./mqtt/MQTTSubscribeServer.o \
./mqtt/MQTTUnsubscribeClient.o \
./mqtt/MQTTUnsubscribeServer.o \
./mqtt/transport.o

C_DEPS += \
./mqtt/MQTTConnectClient.d \
./mqtt/MQTTConnectServer.d \
./mqtt/MQTTDeserializePublish.d \
./mqtt/MQTTFormat.d \
./mqtt/MQTTPacket.d \
./mqtt/MQTTSerializePublish.d \
./mqtt/MQTTSubscribeClient.d \
./mqtt/MQTTSubscribeServer.d \
./mqtt/MQTTUnsubscribeClient.d \
./mqtt/MQTTUnsubscribeServer.d \
./mqtt/transport.d

# Each subdirectory must supply rules for building sources it contributes
mqtt/%.o: ../mqtt/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

