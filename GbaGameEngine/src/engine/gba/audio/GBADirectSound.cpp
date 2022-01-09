#include "GBADirectSound.h"
#include "engine/gba/registers/RegisterMap.h"

void GBA::Audio::DirectSound::SetChannelOptions(Channels channel, int flags, VolumeOptions vol, Timer timer)
{
	u16 volume = 0;
	switch (vol)
	{
		case Vol100:
		{
			volume = BIT(0);
			break;
		}
		case Vol50:
		default:
		{
			volume = 0;
			break;
		}
	}

	int channelFlagBitsOffset = 0;
	int volumeFlagBitsOffset = 0;
	switch (channel)
	{
		case ChannelB:
		{
			channelFlagBitsOffset = 0xC;
			volumeFlagBitsOffset = 0x3;
			break;
		}

		case ChannelA:
		default:
		{
			channelFlagBitsOffset = 0x8;
			volumeFlagBitsOffset = 0x2;
			break;
		}
	}

	u16 options = (volume << volumeFlagBitsOffset) | (flags << channelFlagBitsOffset) | (timer << channelFlagBitsOffset);

	
	vu16& regSoundControl = *(vu16*)(REG_SND_DIRECTSOUNDCONTROL);
	u16 dmgBits = regSoundControl & 0b11;		// DMG volume is set for the first two bits. Don't trash these!
	regSoundControl = dmgBits | options;
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
