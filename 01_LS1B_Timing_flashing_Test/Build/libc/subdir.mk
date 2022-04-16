#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../libc/libc.c \
../libc/lwmem.c \
../libc/printf.c \
../libc/putchar.c \
../libc/qsort.c

OBJS += \
./libc/libc.o \
./libc/lwmem.o \
./libc/printf.o \
./libc/putchar.o \
./libc/qsort.o

C_DEPS += \
./libc/libc.d \
./libc/lwmem.d \
./libc/printf.d \
./libc/putchar.d \
./libc/qsort.d

# Each subdirectory must supply rules for building sources it contributes
libc/%.o: ../libc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

