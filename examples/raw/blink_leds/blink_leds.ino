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

#define BRIGHTNESS 10

#define BLINK_INTERVAL 500

#pragma endregion

#pragma region Headers

#include "made4home.h"
#include "FxTimer.h"

#include <Adafruit_NeoPixel.h> // Click here to get the library: http://librarymanager/All#Adafruit_NeoPixel

#pragma endregion

#pragma region Variables

/** 
 * @brief Blink timer instance.
 */
FxTimer *BlinkTimer_g;

/** 
 * @brief Blink timer instance.
 */
Adafruit_NeoPixel *LEDs_g;

/** 
 * @brief Blink LED state counter.
 */
uint8_t State_g;

#pragma endregion

void setup()
{
	// Setup the blink timer.
	BlinkTimer_g = new FxTimer();
	BlinkTimer_g->setExpirationTime(BLINK_INTERVAL);
	BlinkTimer_g->updateLastTime();

    // Create the object.
    LEDs_g = new Adafruit_NeoPixel(LED_COUNT, PIN_LEDs, NEO_GRB + NEO_KHZ800);
    
    // Init NeoPixel strip object (REQUIRED)
    LEDs_g->begin();
    LEDs_g->show();

    // Set brightness to about. (max = 255)
    LEDs_g->setBrightness(BRIGHTNESS);
    LEDs_g->setPixelColor(0, 0, 0, 0);
    LEDs_g->setPixelColor(1, 0, 0, 0);
    LEDs_g->show();

    // Reset the state.
    State_g = 0;
}

void loop()
{
  BlinkTimer_g->update();
  if(BlinkTimer_g->expired())
  {
    BlinkTimer_g->updateLastTime();
    BlinkTimer_g->clear();

    // set the LED with the StateStatusLED_g of the variable:
    if (State_g == 0)
    {
        LEDs_g->setPixelColor(0, 0, 0, 0);
        LEDs_g->setPixelColor(1, 0, 0, 0);
    }
    else if (State_g == 1)
    {
        LEDs_g->setPixelColor(0, 255, 0, 0);
        LEDs_g->setPixelColor(1, 255, 0, 0);
    }
    else if (State_g == 2)
    {
        LEDs_g->setPixelColor(0, 0, 255, 0);
        LEDs_g->setPixelColor(1, 0, 255, 0);
    }
    else if (State_g == 3)
    {
        LEDs_g->setPixelColor(0, 0, 0, 255);
        LEDs_g->setPixelColor(1, 0, 0, 255);
    }
    else if (State_g == 4)
    {
        LEDs_g->setPixelColor(0, 255, 255, 255);
        LEDs_g->setPixelColor(1, 255, 255, 255);
    }
    else
    {
        LEDs_g->setPixelColor(0, 0, 0, 0);
        LEDs_g->setPixelColor(1, 0, 0, 0);
        State_g = 0;
    }

    LEDs_g->show();
    State_g++;
  }
}
