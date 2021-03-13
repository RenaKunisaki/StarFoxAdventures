# sub-makefile for individual patches
cur_makefile := $(abspath $(lastword $(MAKEFILE_LIST)))
PWD := $(patsubst %/,%,$(dir $(cur_makefile)))

include $(PWD)/config.mk

LINKSCRIPTS := -T$(PWD)/include/gc/gc.ld \
	-T$(PWD)/include/$(GAME)/butt.ld
	#-T$(PWD)/include/$(GAME)/$(GAME)-$(VERSION).ld

#SYMFILE := $(BUILDDIR)/symbols-generated.sym
SYMFILE := $(PWD)/include/$(GANE)/$(GAME)-$(VERSION).sym
OUTPUT  := $(BUILDDIR)/$(NAME).elf

CFILES := $(wildcard $(PWD)/$(NAME)/*.c)
SFILES := $(wildcard $(PWD)/$(NAME)/*.s)
OBJS ?= $(CFILES:.c=.o) $(SFILES:.s=.o)

GCLIB := $(PWD)/include/gc/lib

.PHONY: all clean

all: $(OUTPUT)
	@echo done

clean:
	$(DELETE) $(BUILDDIR)

$(OUTPUT): $(OBJS)
	@echo "[*] Linking: $(OBJS)"
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
