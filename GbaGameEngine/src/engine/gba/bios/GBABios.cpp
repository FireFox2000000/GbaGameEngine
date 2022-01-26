#include "GBABios.h"
#include "engine/base/Macros.h"
#include "engine/gba/interrupts/GBAInterrupts.h"

/*
enum BiosFunctions
{
	SoftReset,
	RegisterRamReset,
	Halt,
	Stop,
	IntrWait,
	VBlankIntrWait,
	Div,
	DivArm,
	Sqrt,
	ArcTan,
	ArcTan2,
	CPUSet,
	CPUFastSet,
	BiosChecksum,
	BgAffineSet,
	ObjAffineSet,
	BitUnPack,
	LZ77UnCompWRAM,
	LZ77UnCompVRAM,
	HuffUnComp,
	RLUnCompWRAM,
	RLUnCompVRAM,
	Diff8bitUnFilterWRAM,
	Diff8bitUnFilterVRAM,
	Diff16bitUnFilter,
	SoundBiasChange,
	SoundDriverInit,
	SoundDriverMode,
	SoundDriverMain,
	SoundDriverVSync,
	SoundChannelClear,
	MIDIKey2Freq,
	MusicPlayerOpen,
	MusicPlayerStart,
	MusicPlayerStop,
	MusicPlayerContinue,
	MusicPlayerFadeOut,
	MultiBoot,
	HardReset,
	CustomHalt,
	SoundDriverVSyncOff,
	SoundDriverVSyncOn,
	GetJumpList,

	Count
};
*/

// Don't let the compiler optimise seemingly empty bios call statements away
#pragma GCC push_options
#pragma GCC optimize ("O0")

void GBA::Bios::VBlankIntrWait()
{
	DEBUG_ASSERTMSG(Interrupts::IsInterruptEnabled(Interrupts::VBlank), "VBlank interrupt not enabled for VBlankIntrWait");

	BiosSystemCall(0x05);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"

int GBA::Bios::Div(int num, int denom)
{
	BiosSystemCall(0x06);
}

void GBA::Bios::SoftReset()
{
	BiosSystemCall(0x00);
}

u16 GBA::Bios::Sqrt(u32 val)
{
	BiosSystemCall(0x08);
}

#pragma GCC diagnostic pop

#pragma GCC pop_options
