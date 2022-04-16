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
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

