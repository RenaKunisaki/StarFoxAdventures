# config variables used by main makefile as well as elf.mk
# edit as needed

GAME ?= sfa
VERSION ?= kd

DISCROOT ?= /home/rena/projects/sfa/kiosk-default/files
#DISCROOT ?= ./discroot/files

TOOL=${DEVKITPRO}/devkitPPC/bin/powerpc-eabi-
SRCDIR ?= .
# XXX can't move build dir because of symbols.s
BUILDDIR ?= $(PWD)/build
#BINDIR ?= bin
GCBASE=$(PWD)/include/gc

CC=@$(TOOL)gcc
AS=@$(TOOL)as
AR=@$(TOOL)ar
LD=@$(TOOL)gcc
NM=@$(TOOL)nm
OBJCOPY=@$(TOOL)objcopy
OBJDUMP=@$(TOOL)objdump
READELF=@$(TOOL)readelf
SIZE=@$(TOOL)size
DELETE=@rm -rf
MKDIR=@mkdir -p

GLOBALSYMS = include/$(GAME)/$(GAME)-$(VERSION).sym
CFLAGS += -mcpu=750 -mhard-float -ffast-math -fno-math-errno -fsingle-precision-constant -shared -fpic -fno-exceptions -fno-asynchronous-unwind-tables -mrelocatable -fno-builtin -G0 -Os -I$(GCBASE) -Iinclude -Iinclude/gc -I$(BUILDDIR)
ASFLAGS += -I include -mbroadway -mregnames -mrelocatable --fatal-warnings
#LDFLAGS += -nostdlib -Wl,--strip-all -export-dynamic $(LINKSCRIPTS) -Wl,--nmagic -Wl,--no-gc-sections -Wl,--just-symbols=$(GLOBALSYMS)
LDFLAGS += -nostdlib -mhard-float $(LINKSCRIPTS) -Wl,--nmagic -Wl,--just-symbols=$(GLOBALSYMS)

# function COMPILE(infile, outfile)
COMPILE=@$(CC) $(CFLAGS) -c $1 -o $2

# function ASSEMBLE(infile, outfile)
ASSEMBLE=@$(AS) $(ASFLAGS) $1 -o $2

# function LINK(infile, outfile)
LINK=@$(LD) $1 $(LDFLAGS) -o $2

ELF=$(BUILDDIR)/$(PROJECT).elf
HEX=$(BINDIR)/$(PROJECT).hex
BIN=$(BINDIR)/$(PROJECT).bin
DIRS=$(BINDIR) $(BUILDDIR)

I_HATE_MAKEFILES=1
