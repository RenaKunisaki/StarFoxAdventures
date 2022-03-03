# sub-makefile for individual patches
cur_makefile := $(abspath $(lastword $(MAKEFILE_LIST)))
PWD := $(patsubst %/,%,$(dir $(cur_makefile)))

include $(PWD)/config.mk

LINKSCRIPTS := -T$(INCLUDE)/gc/gc.ld \
	-T$(INCLUDE)/$(GAME)/butt.ld

#SYMFILE := $(BUILDDIR)/symbols-generated.sym
SYMFILE := $(INCLUDE)/$(GANE)/$(GAME)-$(VERSION).sym
OUTPUT  := $(BUILDDIR)/$(NAME).elf

CFILES := $(wildcard $(PWD)/$(NAME)/*.c)
SFILES := $(wildcard $(PWD)/$(NAME)/*.s)
OBJS ?= $(CFILES:.c=.o) $(SFILES:.s=.o)

GCLIB := $(INCLUDE)/gc/lib

.PHONY: all clean

all: $(OUTPUT)
	@echo done

clean:
	$(DELETE) $(BUILDDIR)

$(OUTPUT): $(OBJS)
	@echo "[*] Linking..."
	@$(call LINK, $(OBJS), $(OUTPUT))

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@echo "[*] Compiling: $<"
	@$(call COMPILE, $<, $@)

$(BUILDDIR)/%.o: $(SRCDIR)/%.s
	@echo "[*] Assembling: $<"
	@$(call ASSEMBLE, $<, $@)

$(BUILDDIR)/lib/%.o: $(INCLUDE)/gc/lib/%.c
	@echo "[*] Compiling: $<"
	@$(call COMPILE, $<, $@)
