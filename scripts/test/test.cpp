#include <iostream>
#include <stdint.h>
#include <cstdlib>

// g++ test.cpp -o test -std=c++20

using namespace std;

constexpr float I2S_DATA_FORMAT_MAX_VALUE = 32768.0f;  // 16bit

int main(void)
{
    float sample = -0.3f;
    cout << (uint16_t)((int16_t)(sample * I2S_DATA_FORMAT_MAX_VALUE)) << endl;
    cout << static_cast<uint16_t>(static_cast<int16_t>(sample * I2S_DATA_FORMAT_MAX_VALUE)) << endl;

    system("ls");

    /* code */
    return 0;
}
