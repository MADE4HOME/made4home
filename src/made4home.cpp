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

#include "made4home.h"

/**
 * @brief Setup the IO board.
 *
 */
void Made4Home_t::setup()
{
    // Create the object.
    m_LEDs = new Adafruit_NeoPixel(LED_COUNT, PIN_LEDs, NEO_GRB + NEO_KHZ800);

    // Init NeoPixel strip object (REQUIRED)
    m_LEDs->begin();
    m_LEDs->show();

    // Set brightness to about. (max = 255)
    m_LEDs->setBrightness(LED_BRIGHTNESS);
    m_LEDs->setPixelColor(0, 0, 0, 0);
    m_LEDs->setPixelColor(1, 0, 0, 0);
    m_LEDs->show();

    //
    m_TWIOne = new TwoWire(0);
    m_TWIOne->begin(PIN_SDA_1, PIN_SCL_1);

    //
    m_MCP = new Adafruit_MCP23008();
    //
    if (!m_MCP->begin(IO_EXPANDER_ADDRESS, m_TWIOne))
    {
        Serial.println("MCP23008 Error.");
        for (;;)
        {
            // Freeze
        }
    }

    for (uint8_t pin = 0; pin < PINS_INPUTS_COUNT; pin++)
    {
        m_MCP->pinMode(PINS_RELAYS[pin], OUTPUT);
        m_MCP->pinMode(PINS_INPUTS[pin], INPUT);
    }
}

/**
 * @brief Set the color of the LEDs.
 *
 * @param index Index of the LEDs [0-1]
 * @param r Red color. [0-255]
 * @param g Green color [0-255]
 * @param b Blue color [0-255]
 * @return * void
 */
void Made4Home_t::setLED(int index, int r, int g, int b)
{
    m_LEDs->setPixelColor(index, r, g, b);
    m_LEDs->show();
}

/**
 * @brief Set the color of the L1 LED.
 *
 * @param r Red color. [0-255]
 * @param g Green color [0-255]
 * @param b Blue color [0-255]
 * @return * void
 */
void Made4Home_t::setL1(int r, int g, int b)
{
    m_LEDs->setPixelColor(0, r, g, b);
    m_LEDs->show();
}

/**
 * @brief Set the color of the L2 LED.
 *
 * @param r Red color. [0-255]
 * @param g Green color [0-255]
 * @param b Blue color [0-255]
 * @return * void
 */
void Made4Home_t::setL2(int r, int g, int b)
{
    m_LEDs->setPixelColor(1, r, g, b);
    m_LEDs->show();
}

/**
 * @brief Read the digital inputs of the IO board.
 *
 * @param pin Pin number [PIN_IN_1 .. 4]
 * @return int Returns HIGH or LOW.
 */
int Made4Home_t::digitalRead(int pin)
{
    if (pin < 0 || pin > 3)
    {
        return LOW;
    }

    return !m_MCP->digitalRead(PINS_INPUTS[pin]);
}

/**
 * @brief Write to the relay outputs of the IO board.
 *
 * @param pin Pin number [0 .. 4]
 * @param state HIGH or LOW
 */
void Made4Home_t::digitalWrite(int pin, int state)
{
    if (pin < 0 || pin > 3)
    {
        return;
    }

    m_MCP->digitalWrite(PINS_RELAYS[pin], state);
}

/**
 * @brief Instance of the IO board abstraction.
 *
 */
Made4Home_t Made4Home;
