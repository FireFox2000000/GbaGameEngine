using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace GbaConversionTools.Tools
{
    internal class DmgMidiConverter
    {
        public enum ChannelId
        {
            Channel1,
            Channel2,
            Channel3,
            Channel4,
        }

        [Flags]
        public enum ChannelData
        {
            Index1 = 0,
            Index2 = 1,
            Index3 = 2,

            All = ~0
        }

        public enum Note
        {
            C,
            Cs,
            D,
            Ds,
            E,
            F,
            Fs,
            G,
            Gs,
            A,
            As,
            B,

            Df = Cs,
            Ef = Ds,
            Gf = Fs,
            Af = Gs,
            Bf = As,
        };

        public interface IEventProperties
        {
            ChannelId id { get; }
            ChannelData dataIndex { get; }
            IEventData[] data { get; }
        }

        public struct Event
        {
            public ushort m_deltaTick;
            public IEventProperties m_properties;
        }

        public void Convert(string inputPath, string outputPath, IList<Event> events)
        {
            CppWriter cppWriter = new CppWriter(Path.GetFileName(Path.GetFileNameWithoutExtension(inputPath)), outputPath);

            cppWriter.Write((uint)events.Count);

            foreach (var e in events)
            {
                cppWriter.Write(e.m_deltaTick);
                cppWriter.Write((ushort)((uint)e.m_properties.id | ((uint)e.m_properties.dataIndex << 2)));

                foreach (var eventData in e.m_properties.data)
                {
                    cppWriter.Write(eventData.data);
                }
            }

            Console.WriteLine("Dmg Midi \"" + System.IO.Path.GetFullPath(outputPath) + "\" successfully converted");

            cppWriter.Finalise();
        }

        public interface IEventData
        {
            ushort data { get; }
        }

        public struct SweepRegister : IEventData
        {
            public enum Mode
            {
                Increase,
                Decrease,
            };

            public ushort number;
            public Mode mode;
            public ushort stepTime;
      
            public ushort data
            {
                get { 
                    return (ushort)(
                        number | 
                        ((ushort)mode << 3) | 
                        ((ushort)stepTime) << 4
                        ); 
                }
            }

            //u16 number : 3
			//	, mode : 1			// Decrease if set, otherwise increase of frequency
			//	, stepTime : 3		// The time between sweeps is measured in 128 Hz (not kHz!): Δt = T/128 ms ~= 7.8T ms; if T=0, the sweep is disabled.
			//	;

        };

        public struct SquareSoundRegister : IEventData
        {
            public enum WaveDutyCycle
            {
                Cycle_1_8,  // 1/8
                Cycle_1_4,  // 1/4 
                Cycle_1_2,  // 1/2
                Cycle_3_4,  // 3/4
            };

            public enum EnvelopeStepDirection
            {
                Decrease,
                Increase,
            };

            /*
            // The envelope function allows for fade-ins or fade-outs of the sound
            // Sound length= (64-register value)*(1/256) seconds
            u16 soundLength : 6		// WRITE-ONLY, only works if the channel is timed (Frequency::Sustain::Timed on FrequencyRegister). The length itself is actually (64-soundLength)/256 seconds for a [3.9, 250] ms range.
				, waveDutyCycle : 2 // GBA::DMG::SquareSound::WaveDutyCycle. Ratio between on and of times of the square wave.
				, envelopeStepTime : 3 // Time between envelope changes: Δt = EST/64 s.
				, envelopeDirection : 1 // GBA::DMG::SquareSound::EnvelopeStepDirection. Indicates if the envelope decreases (default/0) or increases (1) with each step.
				, envelopeInitialVolume : 4 // [0-15] Can be considered a volume setting of sorts: 0 is silent and 15 is full volume. Combined with the direction, you can have fade-in and fade-outs; to have a sustaining sound, set initial volume to 15 and an increasing direction.
				;

            /// <summary>
            /// Converts time to soundLength viable value and sets the soundLength register
            /// </summary>
            /// <param name="milliseconds">Range of [3.9, 250] milliseconds</param>
            void SetSoundLengthSeconds(float seconds);
            */

            public ushort soundLength;
            public WaveDutyCycle waveDutyCycle;
            public ushort envelopeStepTime;
            public EnvelopeStepDirection envelopeDirection;
            public ushort envelopeInitialVolume;

            public ushort data
            {
                get
                {
                    return (ushort)(
                        soundLength
                        | ((ushort)waveDutyCycle << 6)
                        | (envelopeStepTime << 8)
                        | ((ushort)envelopeDirection << 11)
                        | (envelopeInitialVolume << 12)
                        );
                }
            }
        };

        public struct FrequencyRegister : IEventData
        {
            /*
            u16 soundRate : 11 // soundRate = 2^11 - 2^17 / frequency, 64Hz - 131Khz. Write-only field.
				, : 3
				, sustain : 1	// If set, the sound plays for as long as the length field (SquareSoundRegister.soundLength) indicates. If clear, the sound plays forever. Note that even if a decaying envelope has reached 0, the sound itself would still be considered on, even if it's silent.
				, reset : 1		// Write-only. Resets the sound to the initial volume (and sweep) settings, i.e use this to play a new sound.
				;
            */

            public enum Sustain
            {
                Hold,   // Sound plays forever
                Timed,  // Sound plays for as long as the length field
            };

            public enum Reset
            {
                Disabled,
                Enabled
            }

            public ushort soundRate;
            public Sustain sustain;
            public Reset reset;

            public ushort data
            {
                get
                {
                    return (ushort)(
                        soundRate
                        | ((ushort)sustain << 14)
                        | ((ushort)reset << 15)
                        );
                }
            }
        };

        public struct EventChannel1 : IEventProperties
        {
            public ChannelId id => ChannelId.Channel1;
            public IEventData[] data => new IEventData[] { sweep, control, frequency };

            public ChannelData dataIndex => ChannelData.All;

            public SweepRegister sweep;
            public SquareSoundRegister control;
            public FrequencyRegister frequency;
        }

        public struct EventChannel1Sweep : IEventProperties
        {
            public ChannelId id => ChannelId.Channel1;
            public IEventData[] data => new IEventData[] { sweep };
            public ChannelData dataIndex => ChannelData.Index1;

            SweepRegister sweep;
        }

        public struct EventChannel1Control : IEventProperties
        {
            public ChannelId id => ChannelId.Channel1;
            public IEventData[] data => new IEventData[] { control };
            public ChannelData dataIndex => ChannelData.Index2;

            SquareSoundRegister control;
        }

        public struct EventChannel1Frequency : IEventProperties
        {
            public ChannelId id => ChannelId.Channel1;
            public IEventData[] data => new IEventData[] { frequency };
            public ChannelData dataIndex => ChannelData.Index3;

            FrequencyRegister frequency;
        }

        public struct EventChannel2 : IEventProperties
        {
            public ChannelId id => ChannelId.Channel2;
            public IEventData[] data => new IEventData[] { control, frequency };
            public ChannelData dataIndex => ChannelData.All;

            public SquareSoundRegister control;
            public FrequencyRegister frequency;
        }

        public struct EventChannel2Control : IEventProperties
        {
            public ChannelId id => ChannelId.Channel2;
            public IEventData[] data => new IEventData[] { control };
            public ChannelData dataIndex => ChannelData.Index1;

            SquareSoundRegister control;
        }

        public struct EventChannel2Frequency : IEventProperties
        {
            public ChannelId id => ChannelId.Channel2;
            public IEventData[] data => new IEventData[] { frequency };
            public ChannelData dataIndex => ChannelData.Index2;

            FrequencyRegister frequency;
        }

        public static double NoteFrequency(Note note, int octave)
        {
            // http://techlib.com/reference/musical_note_frequencies.htm
            // Notes are separated by a factor of 2^(1/12) or 1.059463.
            const double C0Freq = 16.35160;
            const int TotalNotes = 12;
            int numberOfNotesAwayFromBase = octave * TotalNotes + (int)note;
            return C0Freq * Math.Pow(2.0, (numberOfNotesAwayFromBase / 12.00));
        }

        public static uint FrequencyToRate(double frequency)
        {
            return (1 << 11) - (uint)((1 << 17) / (frequency) + 0.5);
        }

        public static ushort NoteToRate(Note note, int octave)
        {
            return (ushort)FrequencyToRate(NoteFrequency(note, octave));
        }
    }
}
