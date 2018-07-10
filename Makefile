#Paths
#-------------------------------------------------------------------------------
CMSIS = driver/CMSIS
_4x7_ETH_DRV = driver/STM32F4x7_ETH_Driver
STD_DRV = driver/STM32F4xx_StdPeriph_Driver
ETH_DRV = driver/STM32_ETH_Driver
USB_DEV_DRV = driver/STM32_USB_Device_Library
USB_HOST_DRV = driver/STM32_USB_HOST_Library
USB_OTG_DRV = driver/STM32_USB_OTG_Driver

#-------------------------------------------------------------------------------

AS = arm-none-eabi-gcc
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
LD = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size
RM = rm

BUILDDIR = build

SOURCES += $(shell find -L src -name '*.c')
SOURCES += $(shell find -L $(STD_DRV) -name '*.c')
SOURCES += $(shell find -L $(USB_DEV_DRV)/Class/cdc -name '*.c')
#SOURCES += $(shell find -L $(USB_DEV_DRV)/Class/dfu -name '*.c')
SOURCES += $(shell find -L $(USB_DEV_DRV)/Class/msc -name '*.c')
SOURCES += $(shell find -L $(USB_DEV_DRV)/Core -name '*.c')
SOURCES += $(shell find -L jpeg -name '*.c')
#SOURCES += $(shell find -L $(CMSIS) -name '*.c')
#SOURCES += $(shell find -L $(USB_OTG_DRV) -name '*.c')

INCLUDES += -Isrc
INCLUDES += -Isrc/FFT
INCLUDES += -Isrc/lcd
INCLUDES += -Isrc/usb_cdc
INCLUDES += -Ijpeg
INCLUDES += -Iinc
INCLUDES += -I$(CMSIS)/Include
INCLUDES += -I$(CMSIS)/Device/ST/STM32F4xx/Include
INCLUDES += -I$(STD_DRV)/inc
INCLUDES += -I$(USB_DEV_DRV)/Class/cdc/inc
INCLUDES += -I$(USB_DEV_DRV)/Class/dfu/inc
INCLUDES += -I$(USB_DEV_DRV)/Class/msc/inc
INCLUDES += -I$(USB_DEV_DRV)/Core/inc
INCLUDES += -I$(USB_OTG_DRV)/inc

# Find header directories
#INC=$(shell find -L $(INCDIR) -name '*.h' -exec dirname {} \; | uniq)
#INCLUDES=$(INC:%=-I%)

STARTUP = startup/startup_stm32f407xx.s
LDSCR_PATH = ld-scripts
LDSCRIPT = $(LDSCR_PATH)/STM32F407VGTx_FLASH.ld

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

#Compiler
#-------------------------------------------------------------------------------
DEFS = -DUSE_STDPERIPH_DRIVER \
	-DSTM32F4XX \
	-DMEDIA_intFLASH \
	-DUSE_USB_OTG_FS \
	-DHSE_VALUE=8000000 \
	-DARM_MATH_CM4

CFLAGS = -O0 -ggdb -Wall \
	-std=gnu99 \
	-mcpu=cortex-m4 -mthumb \
	-mfpu=fpv4-sp-d16 -mfloat-abi=hard \
	$(DEFS) \
	$(INCLUDES)

#ASM
#-------------------------------------------------------------------------------
AFLAGS = -ahls -mapcs-32

#LINKER
#-------------------------------------------------------------------------------
#LIBS = -larm_cortexM4lf_math
LIBS = -larm_cortexM4l_math
LDFLAGS = -T$(LDSCRIPT) \
	-mthumb \
	-mcpu=cortex-m4 \
	-nostdlib \
	-static -L./lib $(LIBS) \
	--specs=nosys.specs

#-------------------------------------------------------------------------------
ELF = $(BUILDDIR)/main.elf
HEX = $(BUILDDIR)/main.hex

#Compiling
#-------------------------------------------------------------------------------
$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@
	@echo "Compiled "$<"!\n"

$(BUILDDIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(AC) -c $(AFLAGS) $< -o $@
	@echo "Assembled "$<"!\n"

#Linking
#-------------------------------------------------------------------------------
$(ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)
	@echo "Linking complete!\n"
	$(SIZE) $(ELF)

#-------------------------------------------------------------------------------
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

###
# Build Rules
.PHONY: all release debug clean

#-------------------------------------------------------------------------------
all: $(ELF)

release: $(ELF)

debug: $(ELF)

test:
	@echo $(SOURCES)

#-------------------------------------------------------------------------------
flash: $(BIN)
	st-flash write $(BIN) 0x8000000

size:
	@echo "---------------------------------------------------"
	@$(SIZE) $(ELF)

clean:
	$(RM) -rf build
