#ifndef LED_MANAGER_HPP_
#define LED_MANAGER_HPP_

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

class LEDManager {
private:
    static LEDManager* instance;
    LEDManager() {}
public:
    LEDManager(const LEDManager& other) = delete;
    LEDManager& operator=(const LEDManager& other) = delete;
    static LEDManager* getInstance() {
        if (instance == nullptr) {
            instance = new LEDManager();
        }
        return instance;
    }

    LEDManagerState state = LEDManagerState::ON_PRESS;

    void init(void);
    void setState(LEDManagerState newState);

    void update(void);
    void updateOnPress(void);
    void updateSequencer(void);
};

}

#endif // FEATURE_LED_MANAGER
#endif // LED_MANAGER_HPP_