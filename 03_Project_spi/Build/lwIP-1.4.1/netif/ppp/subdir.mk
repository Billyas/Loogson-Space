#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../lwIP-1.4.1/netif/ppp/auth.c \
../lwIP-1.4.1/netif/ppp/chap.c \
../lwIP-1.4.1/netif/ppp/chpms.c \
../lwIP-1.4.1/netif/ppp/fsm.c \
../lwIP-1.4.1/netif/ppp/ipcp.c \
../lwIP-1.4.1/netif/ppp/lcp.c \
../lwIP-1.4.1/netif/ppp/magic.c \
../lwIP-1.4.1/netif/ppp/md5.c \
../lwIP-1.4.1/netif/ppp/pap.c \
../lwIP-1.4.1/netif/ppp/ppp.c \
../lwIP-1.4.1/netif/ppp/ppp_oe.c \
../lwIP-1.4.1/netif/ppp/randm.c \
../lwIP-1.4.1/netif/ppp/vj.c

OBJS += \
./lwIP-1.4.1/netif/ppp/auth.o \
./lwIP-1.4.1/netif/ppp/chap.o \
./lwIP-1.4.1/netif/ppp/chpms.o \
./lwIP-1.4.1/netif/ppp/fsm.o \
./lwIP-1.4.1/netif/ppp/ipcp.o \
./lwIP-1.4.1/netif/ppp/lcp.o \
./lwIP-1.4.1/netif/ppp/magic.o \
./lwIP-1.4.1/netif/ppp/md5.o \
./lwIP-1.4.1/netif/ppp/pap.o \
./lwIP-1.4.1/netif/ppp/ppp.o \
./lwIP-1.4.1/netif/ppp/ppp_oe.o \
./lwIP-1.4.1/netif/ppp/randm.o \
./lwIP-1.4.1/netif/ppp/vj.o

C_DEPS += \
./lwIP-1.4.1/netif/ppp/auth.d \
./lwIP-1.4.1/netif/ppp/chap.d \
./lwIP-1.4.1/netif/ppp/chpms.d \
./lwIP-1.4.1/netif/ppp/fsm.d \
./lwIP-1.4.1/netif/ppp/ipcp.d \
./lwIP-1.4.1/netif/ppp/lcp.d \
./lwIP-1.4.1/netif/ppp/magic.d \
./lwIP-1.4.1/netif/ppp/md5.d \
./lwIP-1.4.1/netif/ppp/pap.d \
./lwIP-1.4.1/netif/ppp/ppp.d \
./lwIP-1.4.1/netif/ppp/ppp_oe.d \
./lwIP-1.4.1/netif/ppp/randm.d \
./lwIP-1.4.1/netif/ppp/vj.d

# Each subdirectory must supply rules for building sources it contributes
lwIP-1.4.1/netif/ppp/%.o: ../lwIP-1.4.1/netif/ppp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

