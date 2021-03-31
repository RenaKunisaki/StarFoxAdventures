PATCHES=bootstrap src
DISCROOT ?= /home/rena/projects/sfa/kiosk-default/files
include $(PWD)/config.mk
.PHONY: all clean

all: $(BUILDDIR) $(BUILDDIR)/boot.bin $(patsubst %,$(BUILDDIR)/%.elf,$(PATCHES))
	@echo "[*] Done."

$(BUILDDIR):
	@mkdir $(BUILDDIR)

# generate symbols.s for inclusion in assembler files.
# this lets us reference them without the need for relocation.
$(BUILDDIR)/symbols.s: $(GLOBALSYMS)
	sed -n -E 's/(\S+)\s+=\s+(0x........);/.set \1, \2/p' $< > $@

$(BUILDDIR)/boot.bin: $(BUILDDIR) $(BUILDDIR)/bootstrap.elf
	@$(TOOL)objcopy -O binary $(BUILDDIR)/bootstrap.elf $(BUILDDIR)/boot.bin
	@rm $(BUILDDIR)/bootstrap.elf

$(BUILDDIR)/%.elf: $(BUILDDIR) %/main.c
	@$(MAKE) --eval="NAME=$*" -f elf.mk

$(BUILDDIR)/%.elf: $(BUILDDIR) $(BUILDDIR)/symbols.s %/main.s
	@$(MAKE) --eval="NAME=$*" -f elf.mk

# XXX DOL patch

clean:
	rm -rf $(BUILDDIR)
	find . -name \*.o -delete

# XXX make this generic, and name it boot.bin instead of debug.bin
install: all
	@echo "[*] Installing..."
	@cp $(BUILDDIR)/boot.bin $(DISCROOT)/debug.bin
	@./tools/elf2bin.py $(BUILDDIR) $(DISCROOT)

# dolphin seems to only work with absolute paths for this
run: install
	dolphin-emu -d $(abspath $(DISCROOT)/../sys/main.dol)
