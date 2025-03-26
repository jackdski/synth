#include "ChordInfo.hpp"

#if (FEATURE_CHORD_INFO)

namespace Audio
{

static uint8_t majorStepsFromBaseNote[] = { 0U, 4U, 7U };
static uint8_t minorStepsFromBaseNote[] = { 0U, 3U, 7U };

static const ChordData_S chordLookupTable[static_cast<uint8_t>(ChordType::CHORD_TYPE_COUNT)] =
{
    {3U, majorStepsFromBaseNote},
    {3U, minorStepsFromBaseNote},
};

const ChordData_S chordInfo_getInfo(ChordType type)
{
    return chordLookupTable[static_cast<int>(type)];
}

}

#endif
