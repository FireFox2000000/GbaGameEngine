#include "GBADirectSound.h"
#include "engine/gba/registers/RegisterMap.h"

struct DirectSoundControlRegister
{
	u16 dmgVolumeRatio : 2				// As to why there's DMG bits in direct sound control registers... who knows???
		, directSoundAVolRatio : 1		// 50% if clear; 100% if set
		, directSoundBVolRatio : 1		// 50% if clear; 100% if set
		, : 4
		, directSoundAEnableRight : 1
		, directSoundAEnableLeft : 1
		, directSoundATimer : 1			// Use timer 0 (if clear) or 1 (if set) for DS A
		, directSoundAReset : 1			// Write only. When using DMA for Direct sound, this will cause DMA to reset the FIFO buffer after it's used.
		, directSoundBEnableRight : 1
		, directSoundBEnableLeft : 1
		, directSoundBTimer : 1			// Use timer 0 (if clear) or 1 (if set) for DS A
		, directSoundBReset : 1			// Write only. When using DMA for Direct sound, this will cause DMA to reset the FIFO buffer after it's used.
		;
};

volatile DirectSoundControlRegister& directSoundControl = (*reinterpret_cast<volatile DirectSoundControlRegister*>(REG_SND_DIRECTSOUNDCONTROL));

void GBA::Audio::DirectSound::SetChannelOptions(Channels channel, int flags, VolumeOptions vol, DSoundTimer timer)
{
	switch (channel)
	{
		case ChannelB:
		{
			directSoundControl.directSoundBTimer = timer;
			directSoundControl.directSoundBReset = (flags & FifoReset) != 0;

			directSoundControl.directSoundBVolRatio = vol;
			directSoundControl.directSoundBEnableLeft = (flags & SoundEnabledLeft) != 0;
			directSoundControl.directSoundBEnableRight = (flags & SoundEnabledRight) != 0;

			break;
		}

		case ChannelA:
		default:
		{
			directSoundControl.directSoundATimer = timer;
			directSoundControl.directSoundAReset = (flags & FifoReset) != 0;

			directSoundControl.directSoundAVolRatio = vol;

			directSoundControl.directSoundAEnableLeft = (flags & SoundEnabledLeft) != 0;
			directSoundControl.directSoundAEnableRight = (flags & SoundEnabledRight) != 0;

			break;
		}
	}
}

vu32 * GBA::Audio::DirectSound::GetDestinationBuffer(Channels channel)
{
	// Fifo buffers, 4 samples/4 bytes
	switch (channel)
	{
	case Channels::ChannelA:
		{
			return (vu32*)0x40000A0;
		}
	case Channels::ChannelB:
		{
			return (vu32*)0x40000A4;
		}

	default:
		break;
	}

	DEBUG_ERROR("Direct sound destination buffer not found!");

	return nullptr;
}
