#pragma once
#include "engine/base/Macros.h"
#include "engine/base/Typedefs.h"

// https://www.coranac.com/tonc/text/sndsqr.htm
namespace GBA
{
	namespace Audio
	{
		namespace DirectSound
		{
			enum VolumeOptions
			{
				Vol50,
				Vol100,

				VolumeCount
			};

			enum Flags
			{
				SoundEnabledRight = BIT(0),
				SoundEnabledLeft = BIT(1),
				FifoReset = BIT(3),
			};

			enum Timer
			{
				Sound0 = 0,
				Sound1 = BIT(2),		// This sneaks inbetween the Flags enum

				SoundTimerCount = 2
			};

			enum Channels
			{
				ChannelA,
				ChannelB,

				ChannelCount
			};

			void SetChannelOptions(Channels channel, int flags, VolumeOptions vol, Timer timer);
			vu32* GetDestinationBuffer(Channels channel);
		}
	}
}
