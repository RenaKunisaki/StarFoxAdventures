GAME ?= sfa
VERSION ?= u0

TOOL=/opt/devkitpro/devkitPPC/bin/powerpc-eabi-
SRCDIR ?= .
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
CFLAGS += -mcpu=750 -mhard-float -ffast-math -fno-math-errno -fsingle-precision-constant -shared -fpic -fno-exceptions -fno-asynchronous-unwind-tables -mrelocatable -fno-builtin -G0 -O3 -I$(GCBASE) -Iinclude -Iinclude/gc -I$(BUILDDIR)
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
