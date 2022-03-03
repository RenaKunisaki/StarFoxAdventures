PATCHES=bootstrap src
include $(PWD)/config.mk

.PHONY: all clean setup install run iso patch setup

ORIGISO ?= original.iso
PATCHISO ?= patched.iso
ORIGDOL = $(DISCROOT)/../sys/main.dol.orig
TOOLS ?= ../amethyst/tools

REPLACE_FILE = LACTIONS.bin
REPLACE_SIZE = 40960

all: $(BUILDDIR) $(BUILDDIR)/dolpatch.bin $(BUILDDIR)/bootstrap.bin $(patsubst %,$(BUILDDIR)/%.elf,$(PATCHES))
	@echo "[*] Done."

$(BUILDDIR): $(ORIGDOL)
	@mkdir -p $(BUILDDIR)
	@mkdir -p patchfiles/sys/
	@mkdir -p patchfiles/files/
	#$(TOOLS)/genSymbols.py $(INCLUDE)/sfa/ build/

# generate symbols.s for inclusion in assembler files.
# this lets us reference them without the need for relocation.
$(BUILDDIR)/symbols.s: $(BUILDDIR) $(GLOBALSYMS)
	@echo "[*] Generate symbols.s"
	@sed -n -E 's/(\S+)\s+=\s+(0x........);/.set \1, \2/p' $(GLOBALSYMS) > $@
	@echo '' >> $@

# build bootstrap file that the game will load at startup
$(BUILDDIR)/bootstrap.bin: $(BUILDDIR) $(BUILDDIR)/bootstrap.elf
	@$(TOOL)objcopy -O binary $(BUILDDIR)/bootstrap.elf $(BUILDDIR)/bootstrap.bin
	#@rm $(BUILDDIR)/bootstrap.elf

$(BUILDDIR)/%.elf: $(BUILDDIR) %/main.c
	@$(MAKE) --eval="NAME=$*" -f elf.mk

$(BUILDDIR)/%.elf: $(BUILDDIR) $(BUILDDIR)/symbols.s %/main.s
	@$(MAKE) --eval="NAME=$*" -f elf.mk

# build patched DOL
$(BUILDDIR)/dolpatch.bin: dolpatch/dolpatch.s $(BUILDDIR)/symbols.s
	@echo "[*] Assembling: $<"
	@$(call ASSEMBLE, $<, $@.elf)
	@echo "[*] Extracting: $@"
	@$(TOOL)objcopy -O binary $@.elf $@
	@echo "[*] Applying DOL patch..."
	@cp $(ORIGDOL) $(BUILDDIR)/main.patched.dol
	@$(TOOLS)/dolpatch.py $@ $(BUILDDIR)/main.patched.dol

# remove build files (but not discroot)
clean:
	rm -rf $(BUILDDIR)
	find . -name \*.o -delete
	@echo "[*] Clean: OK"

# build patch, and install files into discroot.
# ensure replacement file is same size as original so that the game's
# heap patterns are unchanged.
install: all
	@echo "[*] Installing..."
	@cp $(BUILDDIR)/bootstrap.bin $(DISCROOT)/boot.bin
	@cp $(BUILDDIR)/main.patched.dol $(DISCROOT)/../sys/main.dol
	@$(TOOLS)/elf2bin.py $(BUILDDIR) $(DISCROOT)
	@mv $(DISCROOT)/boot.bin $(DISCROOT)/$(REPLACE_FILE)
	@if [ `stat -c '%s' $(DISCROOT)/$(REPLACE_FILE)` -gt $(REPLACE_SIZE) ]; then echo "Patch too large"; exit 1; fi
	@truncate -s $(REPLACE_SIZE) $(DISCROOT)/$(REPLACE_FILE)

# extract ISO
$(ORIGDOL): $(ORIGISO)
	@if test -z "$(DEVKITPRO)"; then echo "DEVKITPRO environment variable is not set"; exit 1; fi
	@echo "[*] Verifying ISO..."
	@md5sum --quiet -c checksums.md5 || (echo "Missing 'original.iso', wrong version, or corrupted file"; exit 1)
	@mkdir -p $(DISCROOT)
	@echo "[*] Extracting ISO..."
	@$(TOOLS)/isobuilder/__main__.py extractIso $(ORIGISO) $(DISCROOT)/../
	@cp $(DISCROOT)/../sys/main.dol $(ORIGDOL)

# build patch and run it
# dolphin seems to only work with absolute paths for this
run: install
	dolphin-emu -d $(abspath $(DISCROOT)/../sys/main.dol)

# build patched ISO
# using bigger compression window (1GB) is necessary to get a patch that's
# ~4MB instead of ~400MB
iso: install
	@echo "[*] Preparing files..."
	@find . -name \*debug.log -delete
	@cp "$(DISCROOT)/$(REPLACE_FILE)" patchfiles/files/$(REPLACE_FILE)
	@cp "$(DISCROOT)/../sys/main.dol" patchfiles/sys/
	@echo "[*] Building ISO..."
	$(TOOLS)/isobuilder/__main__.py overwrite patchIso $(ORIGISO) $(PATCHISO) ./patchfiles

# make patch file
patch: iso
	@echo "[*] Creating patch..."
	@xdelta3 -f -n -B 1073741824 -s $(ORIGISO) $(PATCHISO) patch.xdelta

# set up project (not tested)
setup:
	#@mkdir -p patchfiles/files/gametext
	#@$(TOOLS)/gametext/__main__.py injectAllCharsRecursive $(DISCROOT)/gametext patchfiles/files/gametext/
