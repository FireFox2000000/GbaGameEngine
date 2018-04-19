TARGET := TestGba
CC := gcc
CXX := g++
SRCDIR := src
OBJDIR := obj
BINDIR := bin
DEPDIR := dep
CPPFLAGS := -Wall -pedantic -fno-exceptions -I src/ -O3 #-w -DTEST
LDFLAGS :=

# Recursively search through subdirectories of subdirectories to find files
rfilesearch = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rfilesearch,$d/,$2))

SRCFILES := $(call rfilesearch,$(SRCDIR)/,*.cpp)
OBJFILES := $(SRCFILES:.cpp=.o)
DEPFILES := $(OBJFILES:.o=.d)

ALL_DEPFILES := $(call rfilesearch,$(SRCDIR)/,*.d)
ALL_OBJFILES := $(call rfilesearch,$(SRCDIR)/,*.o)

# Configure for windows commands
ifeq ($(OS),Windows_NT)
	RMCMD = del
	CLEANFILES := $(subst /,\,$(ALL_OBJFILES)) $(subst /,\,$(ALL_DEPFILES))
else
	RMCMD = rm
	CLEANFILES = DEPFILES OBJFILES
endif

all: $(BINDIR)\$(TARGET).gba

# GBA specific compilation
$(BINDIR)\$(TARGET).gba: $(BINDIR)\$(TARGET).elf
	objcopy -O binary $(BINDIR)\$(TARGET).elf $(BINDIR)\$(TARGET).gba

# Pre-GBA compiled binary
$(BINDIR)\$(TARGET).elf: $(OBJFILES)
	$(CXX) -o $@ $^ $(LDFLAGS)
	
-include $(DEPFILES) 

%.d: %.cpp
	@$(CXX) $(CPPFLAGS) $< -MM -MT $(@:.d=.o) >$@
	
clean : 
	$(RMCMD) $(CLEANFILES)
	cd $(BINDIR) && $(RMCMD) *.elf
	$(RMCMD) $(BINDIR)\*.gba