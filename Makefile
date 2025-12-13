###################################
#           PROJECT NAME          
###################################
PROJECT := a7670c

###################################
#           ARCHITECTURE          
###################################
MCU := cortex-m3

###################################
#         STARTUP & LINKER        
###################################
LD_SCRIPT    := stm32_flash.ld
STARTUP_FILE := startup_stm32f10x_md.s

####################################
#            TOOLCHAINS
####################################
TOOLCHAINS_PATH   := /home/bdang/toolchains/gcc-arm-none-eabi-14.3
PREFIX_TOOLCHAINS := arm-none-eabi
CC                := $(TOOLCHAINS_PATH)/bin/$(PREFIX_TOOLCHAINS)-gcc
LD                := $(CC)
CP                := $(TOOLCHAINS_PATH)/bin/$(PREFIX_TOOLCHAINS)-objcopy
SZ                := $(TOOLCHAINS_PATH)/bin/$(PREFIX_TOOLCHAINS)-size
OBJDUMP           := $(TOOLCHAINS_PATH)/bin/$(PREFIX_TOOLCHAINS)-objdump

####################################
#           DIRECTORIES
####################################
BUILD_DIR := z_build

# Source directories
APP_DIR := a_application
MID_DIR := b_middleware
HRD_DIR := c_hardware
THR_DIR := y_thirdparty

# Hardware subdirectories
CMSIS_DIR   := $(HRD_DIR)/CMSIS
DEVICE_DIR  := $(HRD_DIR)/Device
SPL_DIR     := $(HRD_DIR)/SPL

# Middleware subdirectories
DBG_DIR     := $(MID_DIR)/debugger
SPI_DRV_DIR := $(MID_DIR)/spi_driver
TYPEDEF_DIR := $(MID_DIR)/typedef
UART_DRV_DIR := $(MID_DIR)/usart_driver
W25Q_DRV_DIR := $(MID_DIR)/w25q32_driver
GSM_DRV_DIR := $(MID_DIR)/gsm_driver

# Third-party subdirectories
LWRB_DIR    := $(THR_DIR)/lwrb
STR_DIR     := $(THR_DIR)/string_handler
JSMN_DIR 	:= $(THR_DIR)/jsmn

####################################
#           INCLUDE PATHS
####################################
INCDIR := \
	$(APP_DIR) \
	$(APP_DIR)/ota_task \
	$(CMSIS_DIR)/inc \
	$(DEVICE_DIR)/inc \
	$(SPL_DIR)/inc \
	$(TYPEDEF_DIR) \
	$(DBG_DIR) \
	$(SPI_DRV_DIR) \
	$(GSM_DRV_DIR) \
	$(UART_DRV_DIR) \
	$(W25Q_DRV_DIR) \
	$(LWRB_DIR) \
	$(STR_DIR) \
	$(JSMN_DIR) \
	.

####################################
#           SOURCE FILES
####################################
# Application sources
APP_SRC := \
	$(wildcard $(APP_DIR)/*.c) \
	$(wildcard $(APP_DIR)/ota_task/*.c)

# Middleware sources
MID_SRC := \
	$(wildcard $(DBG_DIR)/*.c) \
	$(wildcard $(SPI_DRV_DIR)/*.c) \
	$(wildcard $(UART_DRV_DIR)/*.c) \
	$(wildcard $(W25Q_DRV_DIR)/*.c) \
	$(wildcard $(GSM_DRV_DIR)/*.c)

# Hardware sources - Device
DEV_SRC := \
	$(DEVICE_DIR)/src/a7670c_hardware.c \
	$(DEVICE_DIR)/src/hardware_init.c \
	$(DEVICE_DIR)/src/stm32f1xx_it.c \
	$(DEVICE_DIR)/src/system_stm32f10x.c \
	$(DEVICE_DIR)/src/tiny_printf.c \
	$(DEVICE_DIR)/src/tty_hardware.c \
	$(DEVICE_DIR)/src/w25q32_hardware.c \
	$(DEVICE_DIR)/src/led_hardware.c \
	$(DEVICE_DIR)/src/systick.c

# Hardware sources - SPL (Standard Peripheral Library)
SPL_SRC := \
	$(SPL_DIR)/src/misc.c \
	$(SPL_DIR)/src/stm32f10x_gpio.c \
	$(SPL_DIR)/src/stm32f10x_rcc.c \
	$(SPL_DIR)/src/stm32f10x_usart.c \
	$(SPL_DIR)/src/stm32f10x_spi.c \
	$(SPL_DIR)/src/stm32f10x_dma.c \
	$(SPL_DIR)/src/stm32f10x_flash.c \
	$(SPL_DIR)/src/stm32f10x_exti.c \
	$(SPL_DIR)/src/stm32f10x_tim.c

# Add more SPL sources if needed:
# $(SPL_DIR)/src/stm32f10x_adc.c \
# $(SPL_DIR)/src/stm32f10x_i2c.c \
# $(SPL_DIR)/src/stm32f10x_iwdg.c \
# $(SPL_DIR)/src/stm32f10x_bkp.c \
# $(SPL_DIR)/src/stm32f10x_can.c \
# $(SPL_DIR)/src/stm32f10x_cec.c \
# $(SPL_DIR)/src/stm32f10x_crc.c \
# $(SPL_DIR)/src/stm32f10x_dac.c \
# $(SPL_DIR)/src/stm32f10x_dbgmcu.c \
# $(SPL_DIR)/src/stm32f10x_fsmc.c \
# $(SPL_DIR)/src/stm32f10x_pwr.c \
# $(SPL_DIR)/src/stm32f10x_rtc.c \
# $(SPL_DIR)/src/stm32f10x_sdio.c \
# $(SPL_DIR)/src/stm32f10x_wwdg.c


# Third-party sources
THR_SRC := \
	$(LWRB_DIR)/lwrb.c \
	$(STR_DIR)/string_handler.c

# Root source
ROOT_SRC := main.c

####################################
#            OBJECTS
####################################
# Combine all C sources (FreeRTOS first to avoid implicit declaration)
ALL_SRC := $(RTOS_SRC) $(THR_SRC) $(DEV_SRC) $(SPL_SRC) $(MID_SRC) $(APP_SRC) $(ROOT_SRC)
ALL_OBJ := $(ALL_SRC:%.c=$(BUILD_DIR)/%.o)

# Startup assembly object
STARTUP_OBJ := $(BUILD_DIR)/startup/startup.o

####################################
#              FLAGS
####################################
# Compiler flags
CFLAGS := -mcpu=$(MCU) -mthumb -mthumb-interwork
CFLAGS += -O0 -g3 -Wall -Wextra -Wno-unused-parameter
CFLAGS += -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -fstack-usage
CFLAGS += $(addprefix -I,$(INCDIR))

# Assembler flags
ASFLAGS := -mcpu=$(MCU) -mthumb -mthumb-interwork -g

# Linker flags
LDFLAGS := -mcpu=$(MCU) -mthumb
LDFLAGS += -T$(DEVICE_DIR)/linker/$(LD_SCRIPT)
LDFLAGS += --specs=nano.specs --specs=nosys.specs
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map,--cref
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,--print-memory-usage

####################################
#           BUILD RULES
####################################
.PHONY: all clean flash debug info help

all: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).bin $(BUILD_DIR)/$(PROJECT).hex
	@echo "====================================="
	@echo "Build complete: $(PROJECT)"
	@echo "====================================="

# Generic C file compilation
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Startup assembly file
$(STARTUP_OBJ): $(DEVICE_DIR)/startup/$(STARTUP_FILE)
	@mkdir -p $(BUILD_DIR)/startup
	@echo "[AS] $<"
	@$(CC) $(ASFLAGS) -c $< -o $@

# Linking ELF
$(BUILD_DIR)/$(PROJECT).elf: $(ALL_OBJ) $(STARTUP_OBJ)
	@echo "[LD] Linking $(PROJECT).elf"
	@$(LD) $(ALL_OBJ) $(STARTUP_OBJ) $(LDFLAGS) -o $@
	@echo ""
	@$(SZ) $@
	@echo ""

# Generate BIN
$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	@echo "[BIN] $@"
	@$(CP) -O binary $< $@

# Generate HEX
$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	@echo "[HEX] $@"
	@$(CP) -O ihex $< $@

# Flash using st-flash
flash: $(BUILD_DIR)/$(PROJECT).bin
	@echo "====================================="
	@echo "Flashing $(PROJECT).bin to STM32..."
	@echo "====================================="
	st-flash write $< 0x08004000

# Clean build directory
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)
	@echo "Done."

# Debug with GDB
debug: $(BUILD_DIR)/$(PROJECT).elf
	gdb-multiarch $< \
		-ex "target extended-remote localhost:3333" \
		-ex "monitor reset halt" \
		-ex "load" \
		-ex "break main" \
		-ex "continue"

# Show build info
info:
	@echo "====================================="
	@echo "Project Configuration"
	@echo "====================================="
	@echo "PROJECT      : $(PROJECT)"
	@echo "MCU          : $(MCU)"
	@echo "TOOLCHAIN    : $(TOOLCHAINS_PATH)"
	@echo "LD_SCRIPT    : $(LD_SCRIPT)"
	@echo "BUILD_DIR    : $(BUILD_DIR)"
	@echo ""
	@echo "Source Files:"
	@echo "  APP_SRC    : $(words $(APP_SRC)) files"
	@echo "  MID_SRC    : $(words $(MID_SRC)) files"
	@echo "  SPL_SRC    : $(words $(SPL_SRC)) files"
	@echo "  DEV_SRC    : $(words $(DEV_SRC)) files"
	@echo "  RTOS_SRC   : $(words $(RTOS_SRC)) files"
	@echo "  THR_SRC    : $(words $(THR_SRC)) files"
	@echo "  Total      : $(words $(ALL_SRC)) C files"
	@echo "====================================="

# List all source files (for debugging)
list-sources:
	@echo "====================================="
	@echo "All Source Files:"
	@echo "====================================="
	@echo "Application:"
	@$(foreach src,$(APP_SRC),echo "  $(src)";)
	@echo ""
	@echo "Middleware:"
	@$(foreach src,$(MID_SRC),echo "  $(src)";)
	@echo ""
	@echo "Device:"
	@$(foreach src,$(DEV_SRC),echo "  $(src)";)
	@echo ""
	@echo "SPL:"
	@$(foreach src,$(SPL_SRC),echo "  $(src)";)
	@echo ""
	@echo "FreeRTOS:"
	@$(foreach src,$(RTOS_SRC),echo "  $(src)";)
	@echo ""
	@echo "Third-party:"
	@$(foreach src,$(THR_SRC),echo "  $(src)";)
	@echo ""
	@echo "Root:"
	@echo "  $(ROOT_SRC)"
	@echo "====================================="

# Help
help:
	@echo "====================================="
	@echo "Available targets:"
	@echo "====================================="
	@echo "  all          - Build project (default)"
	@echo "  clean        - Remove build directory"
	@echo "  flash        - Flash binary to STM32"
	@echo "  debug        - Start GDB debug session"
	@echo "  info         - Show project configuration"
	@echo "  list-sources - List all source files"
	@echo "  help         - Show this help message"
	@echo "====================================="