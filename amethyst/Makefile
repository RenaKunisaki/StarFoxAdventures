PATCHES=bootstrap src
DISCROOT ?= /home/rena/projects/sfa/files
include $(PWD)/config.mk
.PHONY: all clean

all: $(BUILDDIR) $(BUILDDIR)/dolpatch.bin $(BUILDDIR)/bootstrap.bin $(patsubst %,$(BUILDDIR)/%.elf,$(PATCHES))
	@echo "[*] Done."

$(BUILDDIR):
	@mkdir $(BUILDDIR)
	#./tools/genSymbols.py include/sfa/ build/

# generate symbols.s for inclusion in assembler files.
# this lets us reference them without the need for relocation.
$(BUILDDIR)/symbols.s: $(GLOBALSYMS)
	@echo "[*] Generate symbols.s"
	@sed -n -E 's/(\S+)\s+=\s+(0x........);/.set \1, \2/p' $< > $@
	@echo '' >> $@

$(BUILDDIR)/bootstrap.bin: $(BUILDDIR) $(BUILDDIR)/bootstrap.elf
	@$(TOOL)objcopy -O binary $(BUILDDIR)/bootstrap.elf $(BUILDDIR)/bootstrap.bin
	#@rm $(BUILDDIR)/bootstrap.elf

$(BUILDDIR)/%.elf: $(BUILDDIR) %/main.c
	@$(MAKE) --eval="NAME=$*" -f elf.mk

$(BUILDDIR)/%.elf: $(BUILDDIR) $(BUILDDIR)/symbols.s %/main.s
	@$(MAKE) --eval="NAME=$*" -f elf.mk

$(BUILDDIR)/dolpatch.bin: dolpatch/dolpatch.s $(BUILDDIR)/symbols.s
	@echo "[*] Assembling: $<"
	@$(call ASSEMBLE, $<, $@.elf)
	@echo "[*] Extracting: $@"
	@$(TOOL)objcopy -O binary $@.elf $@
	@echo "[*] Applying DOL patch..."
	@cp $(DISCROOT)/../sys/main.dol.orig $(BUILDDIR)/main.patched.dol
	@./tools/dolpatch.py $@ $(BUILDDIR)/main.patched.dol

clean:
	rm -rf $(BUILDDIR)
	find . -name \*.o -delete

install: all
	@echo "[*] Installing..."
	@cp $(BUILDDIR)/bootstrap.bin $(DISCROOT)/boot.bin
	@cp $(BUILDDIR)/main.patched.dol $(DISCROOT)/../sys/main.dol
	@./tools/elf2bin.py $(BUILDDIR) $(DISCROOT)
	@./tools/makebitnames.py ../data/U0/gamebits.xml $(DISCROOT)/bitnames.dat

# dolphin seems to only work with absolute paths for this
run: install
	dolphin-emu -d $(abspath $(DISCROOT)/../sys/main.dol)
