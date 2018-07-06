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
SZ = arm-none-eabi-size
RM = rm

BUILDDIR = build

STARTUP = startup/startup_stm32f407xx.s

SOURCES = src \
		$(shell find -L $(CMSIS) -name '*.c') \
		$(shell find -L $(_4x7_ETH_DRV) -name '*.c') \
		$(shell find -L $(STD_DRV) -name '*.c') \
		$(shell find -L $(ETH_DRV) -name '*.c') \
		$(shell find -L $(USB_DEV_DRV) -name '*.c') \
		#$(shell find -L $(USB_HOST_DRV) -name '*.c') \
		$(shell find -L $(USB_OTG_DRV) -name '*.c') \

INCLUDES = -I$(shell find -L $(CMSIS) -name '*.h' -exec dirname {} \; | uniq) \
			-I$(shell find -L $(_4x7_ETH_DRV) -name '*.h' -exec dirname {} \; | uniq) \
			-I$(shell find -L $(STD_DRV) -name '*.h' -exec dirname {} \; | uniq) \
			-I$(shell find -L $(ETH_DRV) -name '*.h' -exec dirname {} \; | uniq) \
			-I$(shell find -L $(USB_DEV_DRV) -name '*.h' -exec dirname {} \; | uniq) \
			#-I$(shell find -L $(USB_HOST_DRV) -name '*.h' -exec dirname {} \; | uniq) \
			-I$(shell find -L $(USB_OTG_DRV) -name '*.h' -exec dirname {} \; | uniq) \
			-Iinc \
			-Ijpeg \
			-Isrc/usb_cdc \
			-Isrc/lcd \
			-Isrc/FFT \




#src
#src/FFT
#src/lcd
#src/usb_cdc
#Jpeg
#inc
#Libraries/CMSIS/Include
#Libraries/CMSIS/Device/ST/STM32F4xx/Include
#Libraries/STM32F4xx_StdPeriph_Driver/inc
#Libraries/STM32_USB_Device_Library/Class/cdc/inc
#Libraries/STM32_USB_Device_Library/Class/dfu/inc
#Libraries/STM32_USB_Device_Library/Class/msc/inc
#Libraries/STM32_USB_Device_Library/Core/inc
#Libraries/STM32_USB_OTG_Driver/inc


# Find header directories
#INC=$(shell find -L $(INCDIR) -name '*.h' -exec dirname {} \; | uniq)
#INCLUDES=$(INC:%=-I%)

LDSCR_PATH = ld-scripts
LDSCRIPT = $(LDSCR_PATH)/STM32F407VGTx_FLASH.ld

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

#Compiler
#-------------------------------------------------------------------------------
DEFS = -DUSE_STDPERIPH_DRIVER \
	-DSTM32F4XX \
	-DMEDIA_intFLASH \
	-DUSE_USB_OTG_FS \
	-DHSE_VALUE=8000000

CFLAGS = -O0 -ggdb -Wall \
	-std=gnu99
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
	-static -L./lib $(LIBS)

#-------------------------------------------------------------------------------
ELF = $(BUILDDIR)/main.elf
HEX = $(BUILDDIR)/main.hex

#-------------------------------------------------------------------------------
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

#Linking
#-------------------------------------------------------------------------------
$(ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

#Building
#-------------------------------------------------------------------------------
$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) -c $(AFLAGS) $< -o $@

#-------------------------------------------------------------------------------
flash: $(BIN)
	st-flash write $(BIN) 0x8000000

size:
	@echo "---------------------------------------------------"
	@$(SZ) $(ELF).elf

clean:
	$(RM) -rf build
