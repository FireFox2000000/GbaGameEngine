using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GbaConversionTools.Tools;

namespace GbaConversionTools.DmgMidis
{
    internal class MidiFallOfFall
    {
        readonly static DmgMidiConverter.EventChannel2 baseNoteEventPrefab = new DmgMidiConverter.EventChannel2()
        {
            control = new DmgMidiConverter.SquareSoundRegister()
            {
                envelopeDirection = DmgMidiConverter.SquareSoundRegister.EnvelopeStepDirection.Decrease,
                envelopeInitialVolume = 15,
                envelopeStepTime = 3,
                soundLength = 0,
                waveDutyCycle = DmgMidiConverter.SquareSoundRegister.WaveDutyCycle.Cycle_3_4
            },
            frequency = new DmgMidiConverter.FrequencyRegister()
            {
                soundRate = DmgMidiConverter.NoteToRate(DmgMidiConverter.Note.G, 5),
                sustain = DmgMidiConverter.FrequencyRegister.Sustain.Hold,
                reset = DmgMidiConverter.FrequencyRegister.Reset.Enabled
            }
        };

        readonly static DmgMidiConverter.EventChannel2 offNoteEventPrefab = new DmgMidiConverter.EventChannel2()
        {
            control = new DmgMidiConverter.SquareSoundRegister()
            {
                envelopeDirection = DmgMidiConverter.SquareSoundRegister.EnvelopeStepDirection.Decrease,
                envelopeInitialVolume = 0,
                envelopeStepTime = 7,
                soundLength = 0,
                waveDutyCycle = DmgMidiConverter.SquareSoundRegister.WaveDutyCycle.Cycle_3_4
            },
            frequency = new DmgMidiConverter.FrequencyRegister()
            {
                soundRate = DmgMidiConverter.NoteToRate(DmgMidiConverter.Note.G, 5),
                sustain = DmgMidiConverter.FrequencyRegister.Sustain.Hold,
                reset = DmgMidiConverter.FrequencyRegister.Reset.Enabled
            }
        };

        static DmgMidiConverter.Event MakeBasicNoteEvent(ushort deltaTick, DmgMidiConverter.EventChannel2 prefab, DmgMidiConverter.Note note, int octave)
        {
            prefab.frequency.soundRate = DmgMidiConverter.NoteToRate(note, octave);
            return new DmgMidiConverter.Event()
            {
                m_deltaTick = deltaTick,
                m_properties = prefab
            };
        }

        public static void MakeMidi(string inputPath, string outputPath)
        {
            DmgMidiConverter converter = new DmgMidiConverter();
            List<DmgMidiConverter.Event> midiEvents = new List<DmgMidiConverter.Event>();

            const int TickStep8 = 15;
            const int TickStep4 = 30;
            const int TickStep2 = 60;

            midiEvents.Add(MakeBasicNoteEvent(TickStep8, baseNoteEventPrefab, DmgMidiConverter.Note.G, 5));
            midiEvents.Add(MakeBasicNoteEvent(TickStep8, baseNoteEventPrefab, DmgMidiConverter.Note.A, 5));
            midiEvents.Add(MakeBasicNoteEvent(TickStep8, baseNoteEventPrefab, DmgMidiConverter.Note.B, 5));
            midiEvents.Add(MakeBasicNoteEvent(TickStep8, baseNoteEventPrefab, DmgMidiConverter.Note.B, 5));
            midiEvents.Add(MakeBasicNoteEvent(TickStep4, baseNoteEventPrefab, DmgMidiConverter.Note.C, 6));
            midiEvents.Add(MakeBasicNoteEvent(TickStep4, baseNoteEventPrefab, DmgMidiConverter.Note.D, 6));
            midiEvents.Add(MakeBasicNoteEvent(TickStep4, baseNoteEventPrefab, DmgMidiConverter.Note.C, 6));
            midiEvents.Add(MakeBasicNoteEvent(TickStep4, baseNoteEventPrefab, DmgMidiConverter.Note.B, 5));
            midiEvents.Add(MakeBasicNoteEvent(TickStep8, baseNoteEventPrefab, DmgMidiConverter.Note.B, 5));
            midiEvents.Add(MakeBasicNoteEvent(TickStep2, baseNoteEventPrefab, DmgMidiConverter.Note.G, 5));
            midiEvents.Add(MakeBasicNoteEvent(TickStep8, baseNoteEventPrefab, DmgMidiConverter.Note.A, 5));
            midiEvents.Add(MakeBasicNoteEvent(TickStep8, baseNoteEventPrefab, DmgMidiConverter.Note.B, 5));
            midiEvents.Add(MakeBasicNoteEvent(TickStep8, baseNoteEventPrefab, DmgMidiConverter.Note.B, 5));
            midiEvents.Add(MakeBasicNoteEvent(TickStep4, baseNoteEventPrefab, DmgMidiConverter.Note.G, 6));
            midiEvents.Add(MakeBasicNoteEvent(TickStep4, baseNoteEventPrefab, DmgMidiConverter.Note.Fs, 6));
            midiEvents.Add(MakeBasicNoteEvent(TickStep4, baseNoteEventPrefab, DmgMidiConverter.Note.B, 5));
            midiEvents.Add(new DmgMidiConverter.Event() { m_deltaTick = TickStep2, m_properties = offNoteEventPrefab });

            converter.Convert(inputPath, outputPath, midiEvents);
        }
    }
}
