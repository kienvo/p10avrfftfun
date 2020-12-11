# Copied PROJECTS/AC14B3K_battery/Makefile
# 11 Dec, 2020

########   EDIT THIS ONLY  #############
DEV = atmega2560
BAUD = 115200
APP = $(shell basename $(abspath $(dir $$PWD)))
SYMBOL = -DBAUD=$(BAUD)
SYMBOL += -DF_CPU=16000000UL 
SYMBOL += -D__ASSERT_USE_STDERR
# SYMBOL += -DFFT_N=(64)
COM?=
########################################

CC = avr-gcc
LD = avr-gcc
AS = avr-gcc

DBGDIR = ./debug
BINDIR	= ./bin
BUILDDIR = ./build
#LIBDIR = /mnt/e/PROJECTS/user_library/avr/lib

#C_LIBSRCS = $(shell find $(LIBDIR) -name '*.c')
#S_LIBSRCS = $(shell find $(LIBDIR) -name '*.S')
#C_LIBSRCS = $(LIBDIR)/utils/DEBUG.c \
			$(LIBDIR)/utils/TWI.c \
			$(LIBDIR)/utils/UART.c
#S_LIBSRCS = 
C_SRCS = $(shell find ./src/ -name '*.c')
S_SRCS = $(shell find ./src/ -name '*.S')

BIN = $(BINDIR)/$(APP)
DBG = $(DBGDIR)/$(APP)
OBJS = $(C_SRCS:./%.c=$(BUILDDIR)/%.o) $(S_SRCS:./%.S=$(BUILDDIR)/%.o) \
$(C_LIBSRCS:$(LIBDIR)/%.c=$(BUILDDIR)/lib/%.o) $(S_LIBSRCS:$(LIBDIR)/%.S=$(BUILDDIR)/lib/%.o)
#INC = 	$(patsubst %,-I%,$(shell find . -type f -name '*.h' -printf "%h\n" | sort -u)) 
#INC += 	$(patsubst %,-I%,$(shell find $(LIBDIR) -type f -name '*.h' -printf "%h\n" | sort -u))
INC += -I./inc
WARNING = -Wall -Wextra -Werror=format -Wfatal-errors #-Werror=conversion
OPTIMIZE = -g2 -O1 -ffunction-sections -fdata-sections -fpack-struct
#OPTIMIZE = -g1 -O3 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax


CFLAGS = -c -std=gnu99 -fsigned-char -MMD $(INC) $(WARNING) $(SYMBOL) $(OPTIMIZE) -mmcu=$(DEV)
ASFLAGS = -c -MMD $(INC) -mmcu=$(DEV) -x assembler-with-cpp
ASFLAGS += -Wa,-adhlns=$(<:.S=.lst),-gstabs 
LDFLAGS = -fsigned-char -Xlinker --gc-sections -Xlinker -Map=$(DBG).map $(OPTIMIZE) -mmcu=$(DEV)


.PHONY: all clean flash rebuild test uart reset
all: post-build

post-build: main-build
	@echo POST
main-build: pre-build
	@$(MAKE) --no-print-directory target
pre-build:
	@clear


target: $(BIN).elf $(BIN).hex $(DBG).lss $(APP).sz

$(BIN).elf: $(OBJS)
	@echo Linking object files ...
	@mkdir -pv $(dir $@)
	@mkdir -pv $(DBGDIR)
	@$(LD) $(LDFLAGS) -o $@ $^
$(BIN).hex: $(BIN).elf 
	@echo Creating hex ...
	@mkdir -pv $(dir $@)
	@avr-objcopy -O ihex $< $@
$(DBG).lss: $(BIN).elf
	@echo Creating .lss ...
	@mkdir -pv $(dir $@)
	@avr-objdump -S $< > $@
$(APP).sz: $(BIN).elf
	@avr-size $< -C --mcu=$(DEV)


$(BUILDDIR)/lib/%.o: lib/%.c
	@echo Compiling $< ...
	@mkdir -pv $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $<
$(BUILDDIR)/lib/%.o: lib/%.S
	@echo Compiling $< ...
	@mkdir -pv $(dir $@)
	@$(AS) $(ASFLAGS) -o $@ $<
	
$(BUILDDIR)/lib/%.o: $(LIBDIR)/%.c
	@echo Compiling $< ...
	@mkdir -pv $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $<
$(BUILDDIR)/lib/%.o: $(LIBDIR)/%.S
	@echo Compiling $< ...
	@mkdir -pv $(dir $@)
	@$(AS) $(ASFLAGS) -o $@ $<
	
$(BUILDDIR)/src/%.o: src/%.c
	@echo Compiling $< ...
	@mkdir -pv $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $<
$(BUILDDIR)/src/%.o: src/%.S
	@echo Compiling $< ...
	@mkdir -pv $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	@rm -vrf $(OBJS) $(OBJS:%.o=%.d) $(BINDIR) $(DBGDIR) $(BUILDDIR)

flash: all
	 avrdude -cusbasp -p$(DEV) -U flash:w:$(BIN).hex:i -v
arduino: all
	avrdude -v -patmega2560 -c wiring -P /dev/ttyACM0 -b $(BAUD) -D -U flash:w:$(BIN).hex:i
reset: 
	avrdude -v -patmega2560 -c wiring -P /dev/ttyACM0
uart: arduino
	putty /dev/ttyACM0 -serial -sercfg 38400,8,n,1,N
test:
	@echo $(C_SRCS)
	@echo $(S_SRCS)
	@echo $(C_LIBSRCS)
	@echo $(S_LIBSRCS)
	@echo $(OBJS)
	@echo $(INC)
rebuild: clean all
cls:
	clear
-include $(OBJS:%.o=%.d)