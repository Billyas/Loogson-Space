#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../yaffs2/port/ls1x_nand_ecc.c \
../yaffs2/port/ls1x_yaffs.c \
../yaffs2/port/yaffs_osglue.c

OBJS += \
./yaffs2/port/ls1x_nand_ecc.o \
./yaffs2/port/ls1x_yaffs.o \
./yaffs2/port/yaffs_osglue.o

C_DEPS += \
./yaffs2/port/ls1x_nand_ecc.d \
./yaffs2/port/ls1x_yaffs.d \
./yaffs2/port/yaffs_osglue.d

# Each subdirectory must supply rules for building sources it contributes
yaffs2/port/%.o: ../yaffs2/port/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

