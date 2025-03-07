
/* I N C L U D E S */

#include "features.h"

#include "MIDI.hpp"

#if FEATURE_MIDI

namespace Audio
{
    namespace MIDI
    {

    /* D E F I N E S */

    /* T Y P E D E F S */

    /* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

    /* P R I V A T E   D A T A   D E F I N I T I O N S */

    static char * noteString[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

    /* P R I V A T E   F U N C T I O N S */


    /* P U B L I C   F U N C T I O N S */

        void midi_getNoteName(const uint32_t midiNumber, char * str)
        {
            // uint32_t octave = (midiNumber / 12) - 1;
            const uint32_t noteIndex = (midiNumber % 12);
            str = noteString[noteIndex];
        }

    }
}

#endif  // FEATURE_MIDI
