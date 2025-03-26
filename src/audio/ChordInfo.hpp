#ifndef CHORD_INFO_HPP
#define CHORD_INFO_HPP

#include "features.h"

#ifndef FEATURE_CHORD_INFO
#define FEATURE_CHORD_INFO 0
#endif

#if (FEATURE_CHORD_INFO)

/* I N C L U D E S */

#include <float.h>
#include <stdint.h>

namespace Audio
{

enum class ChordType
{
    MAJOR,
    MINOR,
    // MAJOR_SEVENTH,
    // MINOR_SEVENTH,
    // DIMINISHED,
    CHORD_TYPE_COUNT
};

struct ChordData_S
{
    uint8_t numNotes;
    uint8_t * stepsFromBaseNote;
};

/* D E F I N E S */


/* T Y P E D E F S */

/* P U B L I C   F U N C T I O N S */
const ChordData_S chordInfo_getInfo(ChordType type);

}

#endif // FEATURE_CHORD_INFO
#endif // CHORD_INFO_HPP