# PIC32 device number
DEVICE		= 32MX320F128H

# UART settings for programmer
TTYDEV		?=/dev/ttyUSB0
TTYBAUD		?=115200

# Name of the project
PROGNAME	= pong

# Compiler and linker flags
CFLAGS		+= -lm -Iinclude -ffreestanding -march=mips32r2 -msoft-float -Wa,-msoft-float
ASFLAGS		+= -msoft-float
LDFLAGS		+=  -lm -T $(LINKSCRIPT)

# Filenames
ELFFILE		= $(PROGNAME).elf
HEXFILE		= $(PROGNAME).hex

# Linkscript upper case to lower case, prefix 'p' and suffix '.ld' for linker file
LINKSCRIPT	:= p$(shell echo "$(DEVICE)" | tr '[:upper:]' '[:lower:]').ld

# Find all source files automatically
CFILES      = $(shell find src -type f -regex ".*\.c")
ASFILES     = $(shell find src -type f -regex ".*\.S")
SYMSFILES	= $(shell find src -type f -regex ".*\.syms")

# Object file names
OBJFILES    = $(CFILES:.c=.c.o)
OBJFILES    +=$(ASFILES:.S=.S.o)
OBJFILES	+=$(SYMSFILES:.syms=.syms.o)

all: $(HEXFILE)

bulle:
	@echo $(CFILES)

clean: 
	$(RM) $(HEXFILE) $(ELFFILE) $(OBJFILES) *.d

envcheck:
	@echo "$(TARGET)" | grep mcb32 > /dev/null || (\
		echo ""; \
		echo " **************************************************************"; \
		echo " * Make sure you have sourced the cross compiling environment *"; \
		echo " * Do this by issuing:                                        *"; \
		echo " * . /path/to/crosscompiler/environment                       *"; \
		echo " **************************************************************"; \
		echo ""; \
		exit 1)

install: envcheck
	$(TARGET)avrdude -v -p $(shell echo "$(DEVICE)" | tr '[:lower:]' '[:upper:]') -c stk500v2 -P "$(TTYDEV)" -b $(TTYBAUD) -U "flash:w:$(HEXFILE)"

$(ELFFILE): $(OBJFILES) envcheck
	$(CC) $(CFLAGS) -o $@ $(OBJFILES) $(LDFLAGS)

$(HEXFILE): $(ELFFILE) envcheck
	$(TARGET)bin2hex -a $(ELFFILE)

# Compile .c files
%.c.o: %.c envcheck
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile .S files
%.S.o: %.S envcheck
	$(CC) $(CFLAGS) $(ASFILES) -c -o $@ $<

# Compile .syms files
%.syms.o: %.syms
	$(LD) -o $@ -r --just-symbols=$<