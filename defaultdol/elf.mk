# sub-makefile for individual patches
cur_makefile := $(abspath $(lastword $(MAKEFILE_LIST)))
PWD := $(patsubst %/,%,$(dir $(cur_makefile)))
DISCROOT ?= /home/rena/projects/sfa/kiosk-default/files

include $(PWD)/config.mk

LINKSCRIPTS := -T$(PWD)/include/gc/gc.ld \
	-T$(PWD)/include/$(GAME)/butt.ld
	#-T$(PWD)/include/$(GAME)/$(GAME)-$(VERSION).ld

SYMFILE := $(PWD)/include/$(GANE)/$(GAME)-$(VERSION).sym
OUTPUT  := $(BUILDDIR)/$(NAME).elf

CFILES := $(wildcard $(PWD)/$(NAME)/*.c)
SFILES := $(wildcard $(PWD)/$(NAME)/*.s)
OBJS ?= $(CFILES:.c=.o) $(SFILES:.s=.o)

GCLIB := $(PWD)/include/gc/lib

.PHONY: all clean

all: $(OUTPUT) dolpatch
	@echo done

clean:
	$(DELETE) $(BUILDDIR)

dolpatch: $(SRCDIR)/dolpatch.s
	@echo "[*] Making DOL patch..."
	$(TOOL)as -I include -mbroadway -mregnames --fatal-warnings -o $<.elf $<
	$(TOOL)objcopy -O binary $<.elf $<.bin
	@echo "[*] Applying DOL patch..."
	./tools/dolpatch.py $<.bin $(DISCROOT)/../sys/default.dol $(DISCROOT)/../sys/main.dol
	rm $<.elf $<.bin

$(OUTPUT): $(OBJS)
	@echo "[*] Linking..."
	@$(call LINK, $(OBJS), $(OUTPUT))

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@echo "[*] Compiling: $<"
	@$(call COMPILE, $<, $@)

$(BUILDDIR)/%.o: $(SRCDIR)/%.s
	@echo "[*] Assembling: $<"
	@$(call ASSEMBLE, $<, $@)

$(BUILDDIR)/lib/%.o: $(PWD)/include/gc/lib/%.c
	@echo "[*] Compiling: $<"
	@$(call COMPILE, $<, $@)
