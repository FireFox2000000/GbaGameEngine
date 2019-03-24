#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/gba_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# DATA is a list of directories containing binary data
# GRAPHICS is a list of directories containing files to be processed by grit
#
# All directories are specified relative to the project directory where
# the makefile is found
#
#---------------------------------------------------------------------------------
TARGET		:= $(notdir $(CURDIR))
BUILD		:= build
ifneq ($(BUILD),$(notdir $(CURDIR)))
SOURCES		:= $(shell find src -type d)
endif
INCLUDES	:= src
DATA		:=
MUSIC		:=

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

CFLAGS	:=	-g -Wall -O3 -pedantic\
		-mcpu=arm7tdmi -mtune=arm7tdmi\
		$(ARCH)

CFLAGS	+=	$(INCLUDE)

CXXFLAGS	:=	$(CFLAGS) -fno-exceptions -std=c++17

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-g $(ARCH) -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:=


#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
			$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

ifneq ($(strip $(MUSIC)),)
	export AUDIOFILES	:=	$(foreach dir,$(notdir $(wildcard $(MUSIC)/*.*)),$(CURDIR)/$(MUSIC)/$(dir))
	BINFILES += soundbank.bin
endif

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES_BIN := $(addsuffix .o,$(BINFILES))

export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export OFILES := $(OFILES_BIN) $(OFILES_SOURCES)

export HFILES := $(addsuffix .h,$(subst .,_,$(BINFILES)))
export HFILES += $(addsuffix .hpp,$(subst .,_,$(BINFILES)))

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).gba


#---------------------------------------------------------------------------------
else

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

$(OUTPUT).gba	:	$(OUTPUT).elf

$(OUTPUT).elf	:	$(OFILES)

$(OFILES_SOURCES) : $(HFILES)

#---------------------------------------------------------------------------------
# The bin2o rule should be copied and modified
# for each extension used in the data directories
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# rule to build soundbank from music files
#---------------------------------------------------------------------------------
soundbank.bin soundbank.h : $(AUDIOFILES)
#---------------------------------------------------------------------------------
	@mmutil $^ -osoundbank.bin -hsoundbank.h

#---------------------------------------------------------------------------------
# This rule links in binary data with the .bin extension
#---------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)


-include $(DEPSDIR)/*.d
#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------


#TARGET := TestGba
#CC := gcc
#CXX := g++
#SRCDIR := src
#OBJDIR := obj
#BUILD := bin
#DEPDIR := dep
#ARCH	:= #-mthumb -mthumb-interwork
#CFLAGS	:= -g -Wall -O3 \
#		#-mcpu=arm7tdmi -mtune=arm7tdmi\
#		$(ARCH)
#LDFLAGS :=
#
#ifeq ($(DEBUG_PC),1)
#	CXXFLAGS := $(CFLAGS) -pedantic -fno-exceptions -I $(SRCDIR)/ -std=c++0x -fpermissive
#	EXE := exe
#else
#	CXXFLAGS := $(CFLAGS) -pedantic -fno-exceptions -I $(SRCDIR)/ -std=c++0x -fpermissive
#	EXE := gba
#endif
#
## Recursively search through subdirectories of subdirectories to find files
#rfilesearch = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rfilesearch,$d/,$2))
#
#CFILES		:=	$(call rfilesearch,$(SRCDIR)/,*.c)
#CPPFILES	:=	$(call rfilesearch,$(SRCDIR)/,*.cpp)
#SFILES		:=	$(call rfilesearch,$(SRCDIR)/,*.s)
#
#OBJFILES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
#DEPFILES := $(OBJFILES:.o=.d)
#
#ALL_DEPFILES := $(call rfilesearch,$(SRCDIR)/,*.d)
#ALL_OBJFILES := $(call rfilesearch,$(SRCDIR)/,*.o)
#
## Configure for windows commands
#ifeq ($(OS),Windows_NT)
#	RMCMD = rm
#	CLEANFILES := $(subst /,\,$(ALL_OBJFILES)) $(subst /,\,$(ALL_DEPFILES))
#else
#	RMCMD = rm
#	CLEANFILES = DEPFILES OBJFILES
#endif
#
##---------------------------------------------------------------------------------
#
#all: $(BUILD)\$(TARGET).$(EXE)
#
## Build an exe
#ifeq ($(DEBUG_PC),1)
#
#$(BUILD)\$(TARGET).$(EXE): $(OBJFILES)
#	$(CXX) -o $@ $^ $(LDFLAGS)
#
#else
#
#$(BUILD)\$(TARGET).$(EXE): $(BUILD)\$(TARGET).elf
#	objcopy -O binary $(BUILD)\$(TARGET).elf $(BUILD)\$(TARGET).gba
#
## Pre-GBA compiled binary
#$(BUILD)\$(TARGET).elf: $(OBJFILES)
#	$(CXX) -o $@ $^ $(LDFLAGS)
#
#endif
#
#-include $(DEPFILES) 
#
#%.d: %.cpp
#	@$(CXX) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@
#
#clean : 
#	$(RMCMD) $(CLEANFILES)
#	#$(RMCMD) -fr $(TARGET).elf $(TARGET).gba