#pragma once

#include "engine/gba/memory/GBAMemoryLocations.h"

#define REG_DISPCNT	(IO_REGISTERS + 0x0)
#define REG_DISPSTAT (IO_REGISTERS + 0x4)
#define REG_VCOUNT (IO_REGISTERS + 0x6)
#define REG_KEYINPUT (IO_REGISTERS + 0x130)
#define REG_TIMERDATA (IO_REGISTERS + 0x100)
#define REG_TIMERCNT (IO_REGISTERS + 0x102)
