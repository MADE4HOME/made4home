/*

MIT License

Copyright (c) [2023] [MADE4HOME]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma region Definitions

#define UPDATE_INTERVAL_MS 1

#pragma endregion

#pragma region Headers

#include "made4home.h"

#include "FxTimer.h"

#pragma endregion

#pragma region Variables

/** 
 * @brief Update timer instance.
 */
FxTimer *UpdateTimer_g;

#pragma endregion

void setup()
{
    // Run the UART.
    Serial.begin(DEFAULT_BAUDRATE, SERIAL_8N1);
    while (!Serial) {}

    Made4Home.setup();

    // Setup the update timer.
    UpdateTimer_g = new FxTimer();
    UpdateTimer_g->setExpirationTime(UPDATE_INTERVAL_MS);
    UpdateTimer_g->updateLastTime();  
}

void loop()
{
    UpdateTimer_g->update();
    if(UpdateTimer_g->expired())
    {
        UpdateTimer_g->updateLastTime();
        UpdateTimer_g->clear();

        // Update the output states via input states.
        for (uint8_t index = 0; index < PINS_INPUTS_COUNT; index++)
        {
            Made4Home.digitalWrite(index, (0 == Made4Home.digitalRead(index)));
        }
    }
}
