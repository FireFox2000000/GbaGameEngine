#pragma once

#include "engine/gba/memory/GBAMemoryLocations.h"

#define REG_IFBIOS (0x3007FF8)
#define REG_INTERRUPT (0x03007FFC)

#define REG_DISPCNT	(IO_REGISTERS + 0x0)
#define REG_DISPSTAT (IO_REGISTERS + 0x4)

#define REG_VCOUNT (IO_REGISTERS + 0x6)
#define REG_BGCNT	(IO_REGISTERS + 0x8)

#define REG_SND1_SWEEP (IO_REGISTERS + 0x60)
#define REG_SND1_CONTROL (IO_REGISTERS + 0x62)
#define REG_SND1_FREQ (IO_REGISTERS + 0x64)
#define REG_SND2_CONTROL (IO_REGISTERS + 0x68)
#define REG_SND2_FREQ (IO_REGISTERS + 0x6C)
#define REG_SND_DMGCONTROL (IO_REGISTERS + 0x80)
#define REG_SND_DIRECTSOUNDCONTROL (IO_REGISTERS + 0x82)
#define REG_SND_STATUS (IO_REGISTERS + 0x84)

#define REG_DMA (IO_REGISTERS + 0xB0)

#define REG_TIMERDATA (IO_REGISTERS + 0x100)
#define REG_TIMERCNT (IO_REGISTERS + 0x102)

#define REG_KEYINPUT (IO_REGISTERS + 0x130)

// Which interrupts are enabled
#define REG_IE (IO_REGISTERS + 0x200)

// Flag for which interrupts occured
#define REG_IF (IO_REGISTERS + 0x202)

// Master interrupt control
#define REG_IME (IO_REGISTERS + 0x208)
