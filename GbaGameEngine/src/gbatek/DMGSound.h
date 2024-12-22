#pragma once
#include "internal/Internal.h"

namespace GBATEK
{
	enum class SweepFrequencyDirection : unsigned short
	{
		Increase,
		Decrease
	};

	enum class WaveDutyCycle : unsigned short
	{
		Cycle_1_8,	// 1/8
		Cycle_1_4,	// 1/4 
		Cycle_1_2,	// 1/2
		Cycle_3_4,	// 3/4
	};

	enum class EnvelopeStepDirection : unsigned short
	{
		Decrease,
		Increase,
	};

	enum class SustainMode : unsigned short
	{
		Forever,
		SoundLength,
	};

	enum class DMGSoundEnable : unsigned short
	{
		Disabled,
		Enabled
	};

	enum class DMGSoundVolume : unsigned short
	{
		Volume25,
		Volume50,
		Volume100
	};

	enum class CounterStep : unsigned short
	{
		Step15,
		Step7,
	};

	enum class SoundReset : unsigned short
	{
		NoReset,
		Reset,
	};

	struct SweepSound
	{
		// (0-7)
		unsigned short number : 3;			
		SweepFrequencyDirection mode : 1;
		
		// (0-7, min=7.8ms, max=54.7ms)
		// The time between sweeps is measured in 128 Hz (not kHz!): ƒ¢t = T/128 ms ~= 7.8T ms; if T=0, the sweep is disabled.
		unsigned short stepTime128Hz : 3;
	};

	struct SquareSound
	{
		// The envelope function allows for fade-ins or fade-outs of the sound
		// Sound length= (64-register value)*(1/256) seconds

		// Write-only. Only works if the channel is timed (Frequency::Sustain::Timed on FrequencyRegister). 
		// The length itself is actually (64-soundLength)/256 seconds for a [3.9, 250] ms range.
		unsigned short soundLength : 6;

		// Ratio between on and of times of the square wave.
		WaveDutyCycle waveDutyCycle : 2;

		// Time between envelope changes: ƒ¢t = EST/64 s.
		unsigned short envelopeStepTime : 3;

		// Indicates if the envelope decreases (default/0) or increases (1) with each step.
		EnvelopeStepDirection envelopeDirection : 1;

		// [0-15] Can be considered a volume setting of sorts: 0 is silent and 15 is full volume. 
		// Combined with the direction, you can have fade-in and fade-outs. 
		// To have a sustaining sound, set initial volume to 15 and an increasing direction.
		unsigned short envelopeInitialVolume : 4;
	};

	struct SquareSoundRead
	{
		unsigned short : 6;

		// Ratio between on and of times of the square wave.
		WaveDutyCycle waveDutyCycle : 2;

		// Time between envelope changes: ƒ¢t = EST/64 s.
		unsigned short envelopeStepTime : 3;

		// Indicates if the envelope decreases (default/0) or increases (1) with each step.
		EnvelopeStepDirection envelopeDirection : 1;

		// [0-15] Can be considered a volume setting of sorts: 0 is silent and 15 is full volume. 
		// Combined with the direction, you can have fade-in and fade-outs. 
		// To have a sustaining sound, set initial volume to 15 and an increasing direction.
		unsigned short envelopeInitialVolume : 4;
	};

	// Frequency sweeps allows "portamento"-like effects where the frequency raises or decreases during playback
	struct SoundFrequency
	{
		// Write-only. SoundRate = 2^11 - 2^17 / frequency, 64Hz - 131Khz.
		unsigned short soundRate : 11;
		unsigned short : 3;

		// If set, the sound plays for as long as the length field (SquareSoundRegister.soundLength) indicates. 
		// If clear, the sound plays forever. Note that even if a decaying envelope has reached 0, the sound itself would still be considered on, even if it's silent.
		SustainMode sustainMode : 1;

		// Write-only. Resets the sound to the initial volume (and sweep) settings, i.e use this to play a new sound.
		SoundReset reset : 1;
	};

	struct SoundFrequencyRead
	{
		// Write-only. SoundRate = 2^11 - 2^17 / frequency, 64Hz - 131Khz.
		unsigned short : 11;
		unsigned short : 3;

		// If set, the sound plays for as long as the length field (SquareSoundRegister.soundLength) indicates. 
		// If clear, the sound plays forever. Note that even if a decaying envelope has reached 0, the sound itself would still be considered on, even if it's silent.
		SustainMode sustainMode : 1;
	};

	struct NoiseControl
	{
		// Write-only. Only works if the channel is timed (Frequency::Sustain::Timed on FrequencyRegister). 
		// The length itself is actually (64-soundLength)/256 seconds for a [3.9, 250] ms range.
		unsigned short soundLength : 6;
		unsigned short : 2;

		// Time between envelope changes: ƒ¢t = EST/64 s.
		unsigned short envelopeStepTime : 3;

		// GBA::DMG::SquareSound::EnvelopeStepDirection. Indicates if the envelope decreases (default/0) or increases (1) with each step.
		EnvelopeStepDirection envelopeDirection : 1;

		// [0-15] Can be considered a volume setting of sorts: 0 is silent and 15 is full volume. Combined with the direction, you can have fade-in and fade-outs; to have a sustaining sound, set initial volume to 15 and an increasing direction.
		unsigned short envelopeInitialVolume : 4;
	};

	struct NoiseControlRead
	{
		unsigned short : 8;

		// Time between envelope changes: ƒ¢t = EST/64 s.
		unsigned short envelopeStepTime : 3;

		// GBA::DMG::SquareSound::EnvelopeStepDirection. Indicates if the envelope decreases (default/0) or increases (1) with each step.
		EnvelopeStepDirection envelopeDirection : 1;

		// [0-15] Can be considered a volume setting of sorts: 0 is silent and 15 is full volume. Combined with the direction, you can have fade-in and fade-outs; to have a sustaining sound, set initial volume to 15 and an increasing direction.
		EnvelopeStepDirection envelopeInitialVolume : 4;
	};

	struct NoiseFrequency
	{
		unsigned short frequencyDividerRatio : 3;

		// Using 7 stages give more metallic sounding effects when played faster (lower divider ratios) while 15 stages sounds much like white noise.
		CounterStep counterStep : 1;
		unsigned short shiftClockFreq : 4;
		unsigned short : 6;

		SustainMode sustainMode : 1;

		// Write-only. All registers can be modified during playback but sound need to be reinitialized when modifying the envelope initial volume or the clock divider for changes to take effects.
		SoundReset reset : 1;
	};

	struct NoiseFrequencyRead
	{
		unsigned short frequencyDividerRatio : 3;

		// Using 7 stages give more metallic sounding effects when played faster (lower divider ratios) while 15 stages sounds much like white noise.
		CounterStep counterStep : 1;
		unsigned short shiftClockFreq : 4;
		unsigned short : 6;

		unsigned short sustain : 1;
	};

	struct DMGSoundChannel1
	{
		SweepSound sweep;
		ReadWriteAccessor<SquareSoundRead, SquareSound> controlRegister;
		ReadWriteAccessor<SoundFrequencyRead, SoundFrequency> frequencyRegister;
	};

	struct DMGSoundChannel2
	{
		ReadWriteAccessor<SquareSoundRead, SquareSound> controlRegister;
		unsigned short : sizeof(unsigned short);
		ReadWriteAccessor<SoundFrequencyRead, SoundFrequency> frequencyRegister;
	};

	struct DMGSoundChannel4
	{
		ReadWriteAccessor<NoiseControlRead, NoiseControl> controlRegister;
		unsigned short : sizeof(unsigned short);
		ReadWriteAccessor<NoiseFrequencyRead, NoiseFrequency> frequencyRegister;
	};

	struct DMGSoundControlRegister
	{
		unsigned short masterVolumeR : 3;		// (0-7)
		unsigned short : 1;
		unsigned short masterVolumeL : 3;		// (0-7)
		unsigned short : 1;
		DMGSoundEnable channel1RightEnabled : 1;
		DMGSoundEnable channel2RightEnabled : 1;
		DMGSoundEnable channel3RightEnabled : 1;
		DMGSoundEnable channel4RightEnabled : 1;
		DMGSoundEnable channel1LeftEnabled : 1;
		DMGSoundEnable channel2LeftEnabled : 1;
		DMGSoundEnable channel3LeftEnabled : 1;
		DMGSoundEnable channel4LeftEnabled : 1;
	};

	static_assert(sizeof(DMGSoundChannel1) == 6, "DMGSoundChannel1 struct malformed");
	static_assert(sizeof(DMGSoundChannel2) == 6, "DMGSoundChannel2 struct malformed");
	static_assert(sizeof(DMGSoundChannel4) == 6, "DMGSoundChannel4 struct malformed");
	static_assert(sizeof(DMGSoundControlRegister) == 2, "DMGSoundControlRegister struct malformed");

	// Tone and sweep
	DMGSoundChannel1* const ioRegisterSoundChannel1 = reinterpret_cast<DMGSoundChannel1*>(0x4000060);

	// Tone
	DMGSoundChannel2* const ioRegisterSoundChannel2 = reinterpret_cast<DMGSoundChannel2*>(0x4000068);

	// Noise
	DMGSoundChannel4* const ioRegisterSoundChannel4 = reinterpret_cast<DMGSoundChannel4*>(0x4000078);

	DMGSoundControlRegister* const ioRegisterSoundControl = reinterpret_cast<DMGSoundControlRegister*>(0x4000080);
}
