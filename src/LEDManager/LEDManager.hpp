#ifndef LED_MANAGER_HPP
#define LED_MANAGER_HPP

#include "features.h"

#if (FEATURE_LED_MANAGER)
#include "LEDs.h"

namespace LED
{

enum class LEDManagerState
{
    INITIALIZING, // could do something fun until UI is initialized
    ON_PRESS,
    // KEYBOARD,
    SEQUENCER,
};

class LEDManager
{
private:
    LEDManagerState state = LEDManagerState::INITIALIZING;

public:
    LEDManager(void);

    void Init(void);
    void Update(void);
    void UpdateOnPress(void);
    void UpdateSequencer(void);
};

LEDManager::LEDManager(void)
{
}

}

#endif // FEATURE_LED_MANAGER
#endif // LED_MANAGER_HPP