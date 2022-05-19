#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../RTT4/components/dfs/src/dfs.c \
../RTT4/components/dfs/src/dfs_file.c \
../RTT4/components/dfs/src/dfs_fs.c \
../RTT4/components/dfs/src/dfs_posix.c \
../RTT4/components/dfs/src/poll.c \
../RTT4/components/dfs/src/select.c

OBJS += \
./RTT4/components/dfs/src/dfs.o \
./RTT4/components/dfs/src/dfs_file.o \
./RTT4/components/dfs/src/dfs_fs.o \
./RTT4/components/dfs/src/dfs_posix.o \
./RTT4/components/dfs/src/poll.o \
./RTT4/components/dfs/src/select.o

C_DEPS += \
./RTT4/components/dfs/src/dfs.d \
./RTT4/components/dfs/src/dfs_file.d \
./RTT4/components/dfs/src/dfs_fs.d \
./RTT4/components/dfs/src/dfs_posix.d \
./RTT4/components/dfs/src/poll.d \
./RTT4/components/dfs/src/select.d

# Each subdirectory must supply rules for building sources it contributes
RTT4/components/dfs/src/%.o: ../RTT4/components/dfs/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

