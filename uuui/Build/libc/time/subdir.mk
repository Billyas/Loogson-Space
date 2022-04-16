#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../libc/time/gmtime.c \
../libc/time/localtime.c \
../libc/time/mktime.c \
../libc/time/strftime.c \
../libc/time/time.c \
../libc/time/tm_utils.c

OBJS += \
./libc/time/gmtime.o \
./libc/time/localtime.o \
./libc/time/mktime.o \
./libc/time/strftime.o \
./libc/time/time.o \
./libc/time/tm_utils.o

C_DEPS += \
./libc/time/gmtime.d \
./libc/time/localtime.d \
./libc/time/mktime.d \
./libc/time/strftime.d \
./libc/time/time.d \
./libc/time/tm_utils.d

# Each subdirectory must supply rules for building sources it contributes
libc/time/%.o: ../libc/time/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RTEMS C Compiler'
	D:/LoongIDE/rtems-4.10/bin/mips-rtems4.10-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe  -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

