PATCHES=bootstrap hello
DISCROOT ?= /home/rena/projects/sfa/files
include $(PWD)/config.mk
.PHONY: all clean

all: $(BUILDDIR) $(BUILDDIR)/boot.bin $(patsubst %,$(BUILDDIR)/%.elf,$(PATCHES))
	@echo "[*] Done."

$(BUILDDIR):
	@mkdir $(BUILDDIR)

$(BUILDDIR)/boot.bin: $(BUILDDIR) $(BUILDDIR)/bootstrap.elf
	@$(TOOL)objcopy -O binary $(BUILDDIR)/bootstrap.elf $(BUILDDIR)/boot.bin
	@rm $(BUILDDIR)/bootstrap.elf

$(BUILDDIR)/%.elf: $(BUILDDIR) %/main.c
	@$(MAKE) --eval="NAME=$*" -f elf.mk

$(BUILDDIR)/%.elf: $(BUILDDIR) %/main.s
	@$(MAKE) --eval="NAME=$*" -f elf.mk

# XXX DOL patch

clean:
	rm -rf $(BUILDDIR)
	find . -name \*.o -delete

# XXX make this generic, and name it boot.bin instead of debug.bin
install: all
	@echo "[*] Installing..."
	cp $(BUILDDIR)/boot.bin $(DISCROOT)/debug.bin
	./elf2bin.py $(DISCROOT) $(BUILDDIR)/*.elf

# dolphin seems to only work with absolute paths for this
run: install
	dolphin-emu -d $(abspath $(DISCROOT)/../sys/main.dol)
