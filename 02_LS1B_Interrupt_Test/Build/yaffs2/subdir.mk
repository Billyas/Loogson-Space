#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../yaffs2/yaffs_allocator.c \
../yaffs2/yaffs_attribs.c \
../yaffs2/yaffs_bitmap.c \
../yaffs2/yaffs_checkptrw.c \
../yaffs2/yaffs_ecc.c \
../yaffs2/yaffs_guts.c \
../yaffs2/yaffs_nameval.c \
../yaffs2/yaffs_nand.c \
../yaffs2/yaffs_packedtags1.c \
../yaffs2/yaffs_packedtags2.c \
../yaffs2/yaffs_summary.c \
../yaffs2/yaffs_tagscompat.c \
../yaffs2/yaffs_verify.c \
../yaffs2/yaffs_yaffs1.c \
../yaffs2/yaffs_yaffs2.c

OBJS += \
./yaffs2/yaffs_allocator.o \
./yaffs2/yaffs_attribs.o \
./yaffs2/yaffs_bitmap.o \
./yaffs2/yaffs_checkptrw.o \
./yaffs2/yaffs_ecc.o \
./yaffs2/yaffs_guts.o \
./yaffs2/yaffs_nameval.o \
./yaffs2/yaffs_nand.o \
./yaffs2/yaffs_packedtags1.o \
./yaffs2/yaffs_packedtags2.o \
./yaffs2/yaffs_summary.o \
./yaffs2/yaffs_tagscompat.o \
./yaffs2/yaffs_verify.o \
./yaffs2/yaffs_yaffs1.o \
./yaffs2/yaffs_yaffs2.o

C_DEPS += \
./yaffs2/yaffs_allocator.d \
./yaffs2/yaffs_attribs.d \
./yaffs2/yaffs_bitmap.d \
./yaffs2/yaffs_checkptrw.d \
./yaffs2/yaffs_ecc.d \
./yaffs2/yaffs_guts.d \
./yaffs2/yaffs_nameval.d \
./yaffs2/yaffs_nand.d \
./yaffs2/yaffs_packedtags1.d \
./yaffs2/yaffs_packedtags2.d \
./yaffs2/yaffs_summary.d \
./yaffs2/yaffs_tagscompat.d \
./yaffs2/yaffs_verify.d \
./yaffs2/yaffs_yaffs1.d \
./yaffs2/yaffs_yaffs2.d

# Each subdirectory must supply rules for building sources it contributes
yaffs2/%.o: ../yaffs2/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDE Lite C Compiler'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

