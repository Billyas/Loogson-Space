#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../yaffs2/direct/yaffsfs.c \
../yaffs2/direct/yaffs_hweight.c \
../yaffs2/direct/yaffs_qsort.c

OBJS += \
./yaffs2/direct/yaffsfs.o \
./yaffs2/direct/yaffs_hweight.o \
./yaffs2/direct/yaffs_qsort.o

C_DEPS += \
./yaffs2/direct/yaffsfs.d \
./yaffs2/direct/yaffs_hweight.d \
./yaffs2/direct/yaffs_qsort.d

# Each subdirectory must supply rules for building sources it contributes
yaffs2/direct/%.o: ../yaffs2/direct/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RTEMS C Compiler'
	D:/LoongIDE/rtems-4.10/bin/mips-rtems4.10-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe  -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

