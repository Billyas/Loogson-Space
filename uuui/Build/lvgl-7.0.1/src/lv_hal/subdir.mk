#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../lvgl-7.0.1/src/lv_hal/lv_hal_disp.c \
../lvgl-7.0.1/src/lv_hal/lv_hal_indev.c \
../lvgl-7.0.1/src/lv_hal/lv_hal_tick.c

OBJS += \
./lvgl-7.0.1/src/lv_hal/lv_hal_disp.o \
./lvgl-7.0.1/src/lv_hal/lv_hal_indev.o \
./lvgl-7.0.1/src/lv_hal/lv_hal_tick.o

C_DEPS += \
./lvgl-7.0.1/src/lv_hal/lv_hal_disp.d \
./lvgl-7.0.1/src/lv_hal/lv_hal_indev.d \
./lvgl-7.0.1/src/lv_hal/lv_hal_tick.d

# Each subdirectory must supply rules for building sources it contributes
lvgl-7.0.1/src/lv_hal/%.o: ../lvgl-7.0.1/src/lv_hal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RTEMS C Compiler'
	D:/LoongIDE/rtems-4.10/bin/mips-rtems4.10-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe  -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

