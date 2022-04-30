#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/asn1_dec.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/asn1_enc.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/mib2.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/mib_structs.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/msg_in.c \
../RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/msg_out.c

OBJS += \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/asn1_dec.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/asn1_enc.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/mib2.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/mib_structs.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/msg_in.o \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/msg_out.o

C_DEPS += \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/asn1_dec.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/asn1_enc.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/mib2.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/mib_structs.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/msg_in.d \
./RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/msg_out.d

# Each subdirectory must supply rules for building sources it contributes
RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/%.o: ../RTT4/components/net/lwip/lwip-1.4.1/src/core/snmp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe  -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

