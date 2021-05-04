MK_DIR = mkdir -p

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
hpl/ramecc \
hpl/gmac \
ethernet_phy \
examples \
hpl/oscctrl \
hpl/sdhc \
hpl/tc \
hpl/osc32kctrl \
lwip/lwip-1.4.0/src/api \
usb \
hpl/dmac \
hal/src \
hpl/eic \
hpl/wdt \
hpl/rtc \
lwip/lwip-1.4.0/src/core/ipv4 \
hpl/usb \
lwip/lwip-1.4.0/src/core \
lwip/lwip-1.4.0/src/netif \
hpl/ccl \
lwip/lwip-1.4.0/port \
gcc \
hpl/pm \
hpl/cmcc \
hpl/gclk \
hal/utils/src \
gcc/gcc \
hpl/mclk \
usb/device \
usb/class/vendor/device \
hpl/pac \
lwip/lwip-1.4.0/src/core/snmp \
hpl/sercom \
hpl/nvmctrl \
hpl/core

# List the object files
OBJS +=  \
hpl/ccl/hpl_ccl.o \
usb/usb_protocol.o \
hal/utils/src/utils_syscalls.o \
lwip/lwip-1.4.0/src/core/ipv4/icmp.o \
hal/src/hal_timer.o \
usb/class/vendor/device/vendordf.o \
lwip/lwip-1.4.0/src/core/def.o \
hpl/pm/hpl_pm.o \
lwip/lwip-1.4.0/src/api/netbuf.o \
lwip/lwip-1.4.0/src/core/sys.o \
hal/src/hal_usart_sync.o \
hpl/ramecc/hpl_ramecc.o \
ethernet_phy/ethernet_phy.o \
hal/src/hal_spi_s_sync.o \
hal/src/hal_usb_device.o \
hpl/rtc/hpl_rtc.o \
lwip/lwip-1.4.0/src/core/ipv4/autoip.o \
gcc/gcc/startup_same53.o \
hal/src/hal_gpio.o \
lwip/lwip-1.4.0/src/core/timers.o \
lwip/lwip-1.4.0/src/api/err.o \
lwip/lwip-1.4.0/src/api/api_msg.o \
hpl/eic/hpl_eic.o \
lwip/lwip-1.4.0/src/core/tcp_out.o \
lwip/lwip-1.4.0/src/core/ipv4/ip_frag.o \
hpl/mclk/hpl_mclk.o \
lwip/lwip-1.4.0/src/core/pbuf.o \
ethernet_phy_main.o \
lwip/lwip-1.4.0/src/core/udp.o \
hal/utils/src/utils_list.o \
hpl/dmac/hpl_dmac.o \
hpl/sdhc/hpl_sdhc.o \
hal/src/hal_flash.o \
hpl/osc32kctrl/hpl_osc32kctrl.o \
lwip/lwip-1.4.0/src/api/netdb.o \
lwip/lwip-1.4.0/src/core/ipv4/ip.o \
driver_init.o \
hal/src/hal_ext_irq.o \
lwip/lwip-1.4.0/src/core/memp.o \
hal/utils/src/utils_event.o \
lwip/lwip-1.4.0/src/core/ipv4/inet_chksum.o \
hpl/cmcc/hpl_cmcc.o \
atmel_start.o \
lwip/lwip-1.4.0/src/netif/etharp.o \
hal/src/hal_atomic.o \
lwip/lwip-1.4.0/src/core/dhcp.o \
hal/src/hal_io.o \
gcc/system_same53.o \
hpl/wdt/hpl_wdt.o \
lwip/lwip-1.4.0/src/core/raw.o \
lwip/lwip-1.4.0/src/core/mem.o \
hpl/core/hpl_core_m4.o \
hpl/nvmctrl/hpl_nvmctrl.o \
lwip/lwip-1.4.0/src/core/tcp.o \
lwip/lwip-1.4.0/port/ethif_mac.o \
lwip/lwip-1.4.0/src/api/api_lib.o \
eth_ipstack_main.o \
hal/src/hal_delay.o \
hpl/core/hpl_init.o \
usb_start.o \
hpl/oscctrl/hpl_oscctrl.o \
hal/src/hal_init.o \
examples/driver_examples.o \
hpl/sercom/hpl_sercom.o \
hal/src/hal_sleep.o \
lwip/lwip-1.4.0/src/core/init.o \
hal/src/hal_mac_async.o \
usb/device/usbdc.o \
hpl/tc/hpl_tc.o \
lwip/lwip-1.4.0/src/core/dns.o \
hpl/pac/hpl_pac.o \
hpl/gmac/hpl_gmac.o \
hal/src/hal_pac.o \
lwip/lwip-1.4.0/src/core/stats.o \
lwip/lwip-1.4.0/src/core/ipv4/igmp.o \
hal/src/hal_spi_m_sync.o \
hal/src/hal_mci_sync.o \
hpl/usb/hpl_usb.o \
hpl/gclk/hpl_gclk.o \
hal/src/hal_wdt.o \
lwip/lwip-1.4.0/src/core/tcp_in.o \
hal/utils/src/utils_assert.o \
lwip/lwip-1.4.0/src/core/ipv4/ip_addr.o \
main.o debug.o fpga.o econet.o timers.o \
lwip/lwip-1.4.0/src/core/ipv4/inet.o \
lwip/lwip-1.4.0/src/api/netifapi.o \
lwip/lwip-1.4.0/src/api/sockets.o \
lwip/lwip-1.4.0/src/core/netif.o \
hal/src/hal_cache.o \
lwip/lwip-1.4.0/src/api/tcpip.o

# List the directories containing header files
DIR_INCLUDES +=  \
-I"../" \
-I"../config" \
-I"../examples" \
-I"../hal/include" \
-I"../hal/utils/include" \
-I"../hpl/ccl" \
-I"../hpl/cmcc" \
-I"../hpl/core" \
-I"../hpl/dmac" \
-I"../hpl/eic" \
-I"../hpl/gclk" \
-I"../hpl/mclk" \
-I"../hpl/nvmctrl" \
-I"../hpl/osc32kctrl" \
-I"../hpl/oscctrl" \
-I"../hpl/pac" \
-I"../hpl/pm" \
-I"../hpl/port" \
-I"../hpl/ramecc" \
-I"../hpl/rtc" \
-I"../hpl/sdhc" \
-I"../hpl/sercom" \
-I"../hpl/tc" \
-I"../hpl/usb" \
-I"../hpl/wdt" \
-I"../hri" \
-I"../" \
-I"../config" \
-I"../ethernet_phy" \
-I"../" \
-I"../config" \
-I"../" \
-I"../config" \
-I"../lwip/lwip-1.4.0/port" \
-I"../lwip/lwip-1.4.0/port/include" \
-I"../lwip/lwip-1.4.0/src/include" \
-I"../lwip/lwip-1.4.0/src/include/ipv4" \
-I"../" \
-I"../config" \
-I"../usb" \
-I"../usb/class/vendor" \
-I"../usb/class/vendor/device" \
-I"../usb/device" \
-I"../" \
-I"../CMSIS/Core/Include" \
-I"../include"

# List the dependency files
DEPS := $(OBJS:%.o=%.d)

OUTPUT_FILE_NAME := eb
QUOTE := "
OUTPUT_FILE_PATH +=$(OUTPUT_FILE_NAME).elf
OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_FILE_NAME).elf

vpath %.c ../
vpath %.s ../
vpath %.S ../

# All Target
all: $(SUB_DIRS) $(OUTPUT_FILE_PATH)

# Linker target
$(OUTPUT_FILE_PATH): $(OBJS)
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -o $(OUTPUT_FILE_NAME).elf $(OBJS) -Wl,--start-group -lm -Wl,--end-group -mthumb \
-Wl,-Map="$(OUTPUT_FILE_NAME).map" --specs=nosys.specs -Wl,--gc-sections -mcpu=cortex-m4 \
 \
-T"../gcc/gcc/same53n19a_flash.ld" \
-L"../gcc/gcc"
	@echo Finished building target: $@

	"arm-none-eabi-objcopy" -O binary "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).bin"
	"arm-none-eabi-objcopy" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  \
        "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).hex"
	"arm-none-eabi-objcopy" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma \
        .eeprom=0 --no-change-warnings -O binary "$(OUTPUT_FILE_NAME).elf" \
        "$(OUTPUT_FILE_NAME).eep" || exit 0
	"arm-none-eabi-objdump" -h -S "$(OUTPUT_FILE_NAME).elf" > "$(OUTPUT_FILE_NAME).lss"
	"arm-none-eabi-size" "$(OUTPUT_FILE_NAME).elf"

# Compiler targets
%.o: %.c
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -g3 -Wall -c -std=gnu99 \
-D__SAME53N19A__ -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"

%.o: %.s
	$(QUOTE)arm-none-eabi-as$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -g3 -Wall -c -std=gnu99 \
-D__SAME53N19A__ -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"

%.o: %.S
	@echo Building file: $<
	@echo ARM/GNU Preprocessing Assembler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x assembler -mthumb -DDEBUG -Os -ffunction-sections -g3 -Wall -c -std=gnu99 \
-D__SAME53N19A__ -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"

# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

$(SUB_DIRS):
	$(MK_DIR) "$@"

fpga_image.h: ../../fpga/sdlc_Implmnt/sbt/outputs/bitmap/top_bitmap.bin Makefile
	od -A none -t x1 -v $< | sed 's/ /, 0x/g' | dd bs=1 skip=2 > $@

main.o: fpga_image.h

clean:
	rm -f $(OBJS_AS_ARGS)
	rm -f $(OUTPUT_FILE_PATH)
	rm -f $(OUTPUT_FILE_NAME).a $(OUTPUT_FILE_NAME).hex $(OUTPUT_FILE_NAME).bin \
        $(OUTPUT_FILE_NAME).lss $(OUTPUT_FILE_NAME).eep $(OUTPUT_FILE_NAME).map \
        $(OUTPUT_FILE_NAME).srec
