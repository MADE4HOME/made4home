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

#define UART0_BAUDRATE 115200

#define UPDATE_INTERVAL 1

#pragma endregion

#pragma region Headers

#include <Wire.h>

#include "made4home.h"
#include "FxTimer.h"

#include <Adafruit_MCP23008.h> // Click here to get the library: http://librarymanager/All#Adafruit_MCP23008

#pragma endregion

#pragma region Variables

/** 
 * @brief Blink timer instance.
 */
FxTimer *UpdateTimer_g;

TwoWire *TWIOne_g;

Adafruit_MCP23008 *MCP_g;

#pragma endregion

void setup()
{
  // Run the UART.
  Serial.begin(UART0_BAUDRATE);

	// Setup the blink timer.
	UpdateTimer_g = new FxTimer();
	UpdateTimer_g->setExpirationTime(UPDATE_INTERVAL);
	UpdateTimer_g->updateLastTime();
  
  //
  TWIOne_g = new TwoWire(0);
  TWIOne_g->begin(PIN_SDA_1, PIN_SCL_1);

  //
  MCP_g = new Adafruit_MCP23008();

  //
  if (!MCP_g->begin(IO_EXPANDER_ADDRESS, TWIOne_g))
  {
    Serial.println("MCP23008 Error.");
    for (;;)
    {
      // Stop
    }
  }

    MCP_g->pinMode(PIN_RELAY_1, OUTPUT);
    MCP_g->pinMode(PIN_RELAY_2, OUTPUT);
    MCP_g->pinMode(PIN_RELAY_3, OUTPUT);
    MCP_g->pinMode(PIN_RELAY_4, OUTPUT);
    MCP_g->pinMode(PIN_IN_1, INPUT);
    MCP_g->pinMode(PIN_IN_2, INPUT);
    MCP_g->pinMode(PIN_IN_3, INPUT);
    MCP_g->pinMode(PIN_IN_4, INPUT);
}

void loop()
{
  UpdateTimer_g->update();
  if(UpdateTimer_g->expired())
  {
    UpdateTimer_g->updateLastTime();
    UpdateTimer_g->clear();

    // Update the output states via input states.
    MCP_g->digitalWrite(PIN_RELAY_1, (0 == MCP_g->digitalRead(PIN_IN_1)));
    MCP_g->digitalWrite(PIN_RELAY_2, (0 == MCP_g->digitalRead(PIN_IN_2)));
    MCP_g->digitalWrite(PIN_RELAY_3, (0 == MCP_g->digitalRead(PIN_IN_3)));
    MCP_g->digitalWrite(PIN_RELAY_4, (0 == MCP_g->digitalRead(PIN_IN_4)));
  }
}
