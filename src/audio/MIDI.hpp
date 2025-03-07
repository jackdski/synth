#ifndef MIDI_HPP_
#define MIDI_HPP_

#include "features.h"

#if (FEATURE_MIDI)

/* I N C L U D E S */

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

namespace Audio
{
    namespace MIDI
    {

        /* D E F I N E S */


        /* T Y P E D E F S */

        /* P U B L I C   F U N C T I O N S */

        void midi_getNoteName(const uint32_t midiNumber, char * str);
    }
}

#endif // FEATURE_MIDI
#endif // MIDI_HPP_