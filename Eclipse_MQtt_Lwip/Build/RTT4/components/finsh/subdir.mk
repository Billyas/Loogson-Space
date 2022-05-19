#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../RTT4/components/finsh/cmd.c \
../RTT4/components/finsh/finsh_compiler.c \
../RTT4/components/finsh/finsh_error.c \
../RTT4/components/finsh/finsh_heap.c \
../RTT4/components/finsh/finsh_init.c \
../RTT4/components/finsh/finsh_node.c \
../RTT4/components/finsh/finsh_ops.c \
../RTT4/components/finsh/finsh_parser.c \
../RTT4/components/finsh/finsh_token.c \
../RTT4/components/finsh/finsh_var.c \
../RTT4/components/finsh/finsh_vm.c \
../RTT4/components/finsh/msh.c \
../RTT4/components/finsh/msh_file.c \
../RTT4/components/finsh/shell.c \
../RTT4/components/finsh/symbol.c

OBJS += \
./RTT4/components/finsh/cmd.o \
./RTT4/components/finsh/finsh_compiler.o \
./RTT4/components/finsh/finsh_error.o \
./RTT4/components/finsh/finsh_heap.o \
./RTT4/components/finsh/finsh_init.o \
./RTT4/components/finsh/finsh_node.o \
./RTT4/components/finsh/finsh_ops.o \
./RTT4/components/finsh/finsh_parser.o \
./RTT4/components/finsh/finsh_token.o \
./RTT4/components/finsh/finsh_var.o \
./RTT4/components/finsh/finsh_vm.o \
./RTT4/components/finsh/msh.o \
./RTT4/components/finsh/msh_file.o \
./RTT4/components/finsh/shell.o \
./RTT4/components/finsh/symbol.o

C_DEPS += \
./RTT4/components/finsh/cmd.d \
./RTT4/components/finsh/finsh_compiler.d \
./RTT4/components/finsh/finsh_error.d \
./RTT4/components/finsh/finsh_heap.d \
./RTT4/components/finsh/finsh_init.d \
./RTT4/components/finsh/finsh_node.d \
./RTT4/components/finsh/finsh_ops.d \
./RTT4/components/finsh/finsh_parser.d \
./RTT4/components/finsh/finsh_token.d \
./RTT4/components/finsh/finsh_var.d \
./RTT4/components/finsh/finsh_vm.d \
./RTT4/components/finsh/msh.d \
./RTT4/components/finsh/msh_file.d \
./RTT4/components/finsh/shell.d \
./RTT4/components/finsh/symbol.d

# Each subdirectory must supply rules for building sources it contributes
RTT4/components/finsh/%.o: ../RTT4/components/finsh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_RTTHREAD  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../RTT4/include" -I"../RTT4/port/include" -I"../RTT4/port/mips" -I"../RTT4/components/finsh" -I"../RTT4/components/dfs/include" -I"../RTT4/components/drivers/include" -I"../RTT4/components/libc/time" -I"../RTT4/bsp-ls1x" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

