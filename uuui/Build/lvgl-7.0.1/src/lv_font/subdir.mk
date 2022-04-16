#
# Auto-Generated file. Do not edit!
#

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../lvgl-7.0.1/src/lv_font/hz_msyh_3500_16.c \
../lvgl-7.0.1/src/lv_font/hz_msyh_3500_24.c \
../lvgl-7.0.1/src/lv_font/hz_simsun_3500_16.c \
../lvgl-7.0.1/src/lv_font/hz_simsun_3500_24.c \
../lvgl-7.0.1/src/lv_font/lv_font.c \
../lvgl-7.0.1/src/lv_font/lv_font_dejavu_16_persian_hebrew.c \
../lvgl-7.0.1/src/lv_font/lv_font_fmt_txt.c \
../lvgl-7.0.1/src/lv_font/lv_font_montserrat_12.c \
../lvgl-7.0.1/src/lv_font/lv_font_montserrat_12_subpx.c \
../lvgl-7.0.1/src/lv_font/lv_font_montserrat_14.c \
../lvgl-7.0.1/src/lv_font/lv_font_montserrat_16.c \
../lvgl-7.0.1/src/lv_font/lv_font_montserrat_18.c \
../lvgl-7.0.1/src/lv_font/lv_font_montserrat_20.c \
../lvgl-7.0.1/src/lv_font/lv_font_montserrat_22.c \
../lvgl-7.0.1/src/lv_font/lv_font_montserrat_24.c \
../lvgl-7.0.1/src/lv_font/lv_font_unscii_8.c

OBJS += \
./lvgl-7.0.1/src/lv_font/hz_msyh_3500_16.o \
./lvgl-7.0.1/src/lv_font/hz_msyh_3500_24.o \
./lvgl-7.0.1/src/lv_font/hz_simsun_3500_16.o \
./lvgl-7.0.1/src/lv_font/hz_simsun_3500_24.o \
./lvgl-7.0.1/src/lv_font/lv_font.o \
./lvgl-7.0.1/src/lv_font/lv_font_dejavu_16_persian_hebrew.o \
./lvgl-7.0.1/src/lv_font/lv_font_fmt_txt.o \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_12.o \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_12_subpx.o \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_14.o \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_16.o \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_18.o \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_20.o \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_22.o \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_24.o \
./lvgl-7.0.1/src/lv_font/lv_font_unscii_8.o

C_DEPS += \
./lvgl-7.0.1/src/lv_font/hz_msyh_3500_16.d \
./lvgl-7.0.1/src/lv_font/hz_msyh_3500_24.d \
./lvgl-7.0.1/src/lv_font/hz_simsun_3500_16.d \
./lvgl-7.0.1/src/lv_font/hz_simsun_3500_24.d \
./lvgl-7.0.1/src/lv_font/lv_font.d \
./lvgl-7.0.1/src/lv_font/lv_font_dejavu_16_persian_hebrew.d \
./lvgl-7.0.1/src/lv_font/lv_font_fmt_txt.d \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_12.d \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_12_subpx.d \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_14.d \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_16.d \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_18.d \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_20.d \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_22.d \
./lvgl-7.0.1/src/lv_font/lv_font_montserrat_24.d \
./lvgl-7.0.1/src/lv_font/lv_font_unscii_8.d

# Each subdirectory must supply rules for building sources it contributes
lvgl-7.0.1/src/lv_font/%.o: ../lvgl-7.0.1/src/lv_font/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RTEMS C Compiler'
	D:/LoongIDE/rtems-4.10/bin/mips-rtems4.10-gcc.exe -mips32 -G0 -EL -msoft-float -DLS1B -DOS_NONE  -O0 -g -Wall -c -fmessage-length=0 -pipe  -I"../" -I"../include" -I"../core/include" -I"../core/mips" -I"../ls1x-drv/include" -I"../yaffs2/direct" -I"../yaffs2/port" -I"../lwIP-1.4.1/include" -I"../lwIP-1.4.1/include/ipv4" -I"../lwIP-1.4.1/port/include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

