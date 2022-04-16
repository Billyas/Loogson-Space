#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../lvgl-7.0.1/src/lv_misc/lv_anim.c \
../lvgl-7.0.1/src/lv_misc/lv_area.c \
../lvgl-7.0.1/src/lv_misc/lv_async.c \
../lvgl-7.0.1/src/lv_misc/lv_bidi.c \
../lvgl-7.0.1/src/lv_misc/lv_color.c \
../lvgl-7.0.1/src/lv_misc/lv_fs.c \
../lvgl-7.0.1/src/lv_misc/lv_gc.c \
../lvgl-7.0.1/src/lv_misc/lv_ll.c \
../lvgl-7.0.1/src/lv_misc/lv_log.c \
../lvgl-7.0.1/src/lv_misc/lv_math.c \
../lvgl-7.0.1/src/lv_misc/lv_mem.c \
../lvgl-7.0.1/src/lv_misc/lv_printf.c \
../lvgl-7.0.1/src/lv_misc/lv_task.c \
../lvgl-7.0.1/src/lv_misc/lv_templ.c \
../lvgl-7.0.1/src/lv_misc/lv_txt.c \
../lvgl-7.0.1/src/lv_misc/lv_txt_ap.c \
../lvgl-7.0.1/src/lv_misc/lv_utils.c

OBJS += \
./lvgl-7.0.1/src/lv_misc/lv_anim.o \
./lvgl-7.0.1/src/lv_misc/lv_area.o \
./lvgl-7.0.1/src/lv_misc/lv_async.o \
./lvgl-7.0.1/src/lv_misc/lv_bidi.o \
./lvgl-7.0.1/src/lv_misc/lv_color.o \
./lvgl-7.0.1/src/lv_misc/lv_fs.o \
./lvgl-7.0.1/src/lv_misc/lv_gc.o \
./lvgl-7.0.1/src/lv_misc/lv_ll.o \
./lvgl-7.0.1/src/lv_misc/lv_log.o \
./lvgl-7.0.1/src/lv_misc/lv_math.o \
./lvgl-7.0.1/src/lv_misc/lv_mem.o \
./lvgl-7.0.1/src/lv_misc/lv_printf.o \
./lvgl-7.0.1/src/lv_misc/lv_task.o \
./lvgl-7.0.1/src/lv_misc/lv_templ.o \
./lvgl-7.0.1/src/lv_misc/lv_txt.o \
./lvgl-7.0.1/src/lv_misc/lv_txt_ap.o \
./lvgl-7.0.1/src/lv_misc/lv_utils.o

C_DEPS += \
./lvgl-7.0.1/src/lv_misc/lv_anim.d \
./lvgl-7.0.1/src/lv_misc/lv_area.d \
./lvgl-7.0.1/src/lv_misc/lv_async.d \
./lvgl-7.0.1/src/lv_misc/lv_bidi.d \
./lvgl-7.0.1/src/lv_misc/lv_color.d \
./lvgl-7.0.1/src/lv_misc/lv_fs.d \
./lvgl-7.0.1/src/lv_misc/lv_gc.d \
./lvgl-7.0.1/src/lv_misc/lv_ll.d \
./lvgl-7.0.1/src/lv_misc/lv_log.d \
./lvgl-7.0.1/src/lv_misc/lv_math.d \
./lvgl-7.0.1/src/lv_misc/lv_mem.d \
./lvgl-7.0.1/src/lv_misc/lv_printf.d \
./lvgl-7.0.1/src/lv_misc/lv_task.d \
./lvgl-7.0.1/src/lv_misc/lv_templ.d \
./lvgl-7.0.1/src/lv_misc/lv_txt.d \
./lvgl-7.0.1/src/lv_misc/lv_txt_ap.d \
./lvgl-7.0.1/src/lv_misc/lv_utils.d

# Each subdirectory must supply rules for building sources it contributes
lvgl-7.0.1/src/lv_misc/%.o: ../lvgl-7.0.1/src/lv_misc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RTEMS C Compiler'
	D:/LoongIDE/rtems-4.10/bin/mips-rtems4.10-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe  -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

