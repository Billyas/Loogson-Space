#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/install_3th_libraries.c \
../src/webclient.c \
../src/webclient_file.c \
../src/webclient_get_sample.c \
../src/webclient_post_sample.c \
../src/webclient_shard_download_sample.c \
../src/ping.c

OBJS += \
./src/install_3th_libraries.o \
./src/webclient.o \
./src/webclient_file.o \
./src/webclient_get_sample.o \
./src/webclient_post_sample.o \
./src/webclient_shard_download_sample.o \
./src/ping.o

C_DEPS += \
./src/install_3th_libraries.d \
./src/webclient.d \
./src/webclient_file.d \
./src/webclient_get_sample.d \
./src/webclient_post_sample.d \
./src/webclient_shard_download_sample.d \
./src/ping.d

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

