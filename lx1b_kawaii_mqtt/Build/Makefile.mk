#
# Auto-generated file. Do not edit!
#

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include objects.mk
-include subdir.mk
-include RTT4/bsp-ls1x/subdir.mk
-include RTT4/bsp-ls1x/i2c/subdir.mk
-include RTT4/bsp-ls1x/spi/subdir.mk
-include RTT4/components/dfs/src/subdir.mk
-include RTT4/components/drivers/src/subdir.mk
-include RTT4/components/finsh/subdir.mk
-include RTT4/components/libc/compilers/newlib/subdir.mk
-include RTT4/components/libc/time/subdir.mk
-include RTT4/components/net/at/at_socket/subdir.mk
-include RTT4/components/net/at/src/subdir.mk
-include RTT4/components/net/netdev/src/subdir.mk
-include RTT4/components/net/sal/dfs_net/subdir.mk
-include RTT4/components/net/sal/impl/subdir.mk
-include RTT4/components/net/sal/socket/subdir.mk
-include RTT4/components/net/sal/src/subdir.mk
-include RTT4/port/subdir.mk
-include RTT4/src/subdir.mk
-include kawaii_mqtt/common/subdir.mk
-include kawaii_mqtt/common/log/subdir.mk
-include kawaii_mqtt/common/log/arch/rt-thread/subdir.mk
-include kawaii_mqtt/common/log/test/subdir.mk
-include kawaii_mqtt/mqtt/subdir.mk
-include kawaii_mqtt/mqttclient/subdir.mk
-include kawaii_mqtt/network/subdir.mk
-include kawaii_mqtt/network/mbedtls/library/subdir.mk
-include kawaii_mqtt/network/mbedtls/wrapper/subdir.mk
-include kawaii_mqtt/platform/RT-Thread/subdir.mk
-include kawaii_mqtt/test/subdir.mk
-include ls1x-drv/subdir.mk
-include ls1x-drv/ac97/subdir.mk
-include ls1x-drv/can/subdir.mk
-include ls1x-drv/console/subdir.mk
-include ls1x-drv/fb/subdir.mk
-include ls1x-drv/fb/font/subdir.mk
-include ls1x-drv/gmac/subdir.mk
-include ls1x-drv/gpio/subdir.mk
-include ls1x-drv/i2c/subdir.mk
-include ls1x-drv/i2c/ads1015/subdir.mk
-include ls1x-drv/i2c/gp7101/subdir.mk
-include ls1x-drv/i2c/gt1151/subdir.mk
-include ls1x-drv/i2c/mcp4725/subdir.mk
-include ls1x-drv/i2c/pca9557/subdir.mk
-include ls1x-drv/i2c/rx8010/subdir.mk
-include ls1x-drv/nand/subdir.mk
-include ls1x-drv/pwm/subdir.mk
-include ls1x-drv/rtc/subdir.mk
-include ls1x-drv/spi/subdir.mk
-include ls1x-drv/spi/w25x40/subdir.mk
-include ls1x-drv/spi/xpt2046/subdir.mk
-include ls1x-drv/uart/subdir.mk
-include ls1x-drv/useful/subdir.mk
-include ls1x-drv/watchdog/subdir.mk
-include lwIP-1.4.1/api/subdir.mk
-include lwIP-1.4.1/core/subdir.mk
-include lwIP-1.4.1/core/ipv4/subdir.mk
-include lwIP-1.4.1/core/snmp/subdir.mk
-include lwIP-1.4.1/netif/subdir.mk
-include lwIP-1.4.1/netif/ppp/subdir.mk
-include lwIP-1.4.1/port/subdir.mk
-include src/subdir.mk
-include yaffs2/subdir.mk
-include yaffs2/direct/subdir.mk
-include yaffs2/port/subdir.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
endif

-include ../makefile.defs

OUT_ELF=lx1b_kawaii_mqtt.exe
OUT_MAP=lx1b_kawaii_mqtt.map
OUT_BIN=lx1b_kawaii_mqtt.bin

LINKCMDS=../ld.script

# Add inputs and outputs from these tool invocations to the build variables

# All Target
all: $(OUT_ELF)

# Tool invocations
$(OUT_ELF): $(STARTO) $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: SDE Lite C Linker'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-gcc.exe -L"%path%/mips-2011.03/lib" -L"%path%/mips-2011.03/mips-sde-elf/lib"  -mips32 -G0 -EL -msoft-float -nostartfiles -static -T $(LINKCMDS) -o $(OUT_ELF) -e start $(STARTO) $(OBJS) $(USER_OBJS) $(LIBS) 
	@echo 'Invoking: SDE Lite C Objcopy'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-objcopy.exe -O binary $(OUT_ELF) $(OUT_BIN)
	@echo 'Invoking: SDE Lite C Size'
	D:/LoongIDE/mips-2011.03/bin/mips-sde-elf-size.exe $(OUT_ELF)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(STARTO) $(OBJS) $(C_DEPS) $(CPP_DEPS) $(ASM_DEPS) $(EXECUTABLES) $(OUT_ELF) $(OUT_MAP) $(OUT_BIN)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets


