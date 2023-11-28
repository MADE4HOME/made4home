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

#ifndef _MADE4HOME_h
#define _MADE4HOME_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#pragma region GPIO Map

// Check the micro controller type.
#if defined(ESP32)

/**
 * @brief Pin of the two LEDs.
 */
#define PIN_LEDs 16

/**
 * @brief Pin of the Relay 1 at the I2C expander.
 */
#define PIN_RELAY_1 4

/**
 * @brief Pin of the Relay 2 at the I2C expander.
 */
#define PIN_RELAY_2 5

/**
 * @brief Pin of the Relay 3 at the I2C expander.
 */
#define PIN_RELAY_3 6

/**
 * @brief Pin of the Relay 4 at the I2C expander.
 */
#define PIN_RELAY_4 7

/**
 * @brief Pin of the Input 1 at the I2C expander.
 */
#define PIN_IN_1 3

/**
 * @brief Pin of the Input 2 at the I2C expander.
 */
#define PIN_IN_2 2

/**
 * @brief Pin of the Input 3 at the I2C expander.
 */
#define PIN_IN_3 1

/**
 * @brief Pin of the Input 4 at the I2C expander.
 */
#define PIN_IN_4 0

/**
 * @brief Pin SDA of the I2C 1.
 */
#define PIN_SDA_1 33

/**
 * @brief Pin SCL of the I2C 1.
 */
#define PIN_SCL_1 32

/**
 * @brief Pin SDA of the I2C 2.
 */
#define PIN_SDA_2 15

/**
 * @brief Pin SCL of the I2C 2.
 */
#define PIN_SCL_2 14

/**
 * @brief Pin enable of the RS485.
 */
#define PIN_RS485_EN 2

/**
 * @brief Pin RX of the RS485.
 */
#define PIN_RS485_RX 39

/**
 * @brief Pin TX of the RS485.
 */
#define PIN_RS485_TX 4

/**
 * @brief Pin ADDR of the Ethernet.
 */
#define PIN_ETH_PHY_ADDR  0

/**
 * @brief Pin POWER of the Ethernet.
 */
#define PIN_ETH_PHY_POWER 5

/**
 * @brief Pin MDC of the Ethernet.
 */
#define PIN_ETH_PHY_MDC   23

/**
 * @brief Pin MDIO of the Ethernet.
 */
#define PIN_ETH_PHY_MDIO  18

/**
 * @brief Pin type of the Ethernet.
 */
#define PIN_ETH_PHY_TYPE ETH_PHY_LAN8720

/**
 * @brief Clock mode of the Ethernet.
 */
#define PIN_ETH_CLK_MODE ETH_CLOCK_GPIO0_IN

#else
#error "Unsupported MCU"

#endif

#pragma endregion

#pragma region Parameters

/**
 * @brief Number of the programable LEDs at the board.
 */
#define LED_COUNT  2

#define LED_BRIGHTNESS 255

#define IO_EXPANDER_ADDRESS 0x20

#pragma endregion

#pragma region Headers

#include <Wire.h>

#include <Adafruit_NeoPixel.h> // Click here to get the library: http://librarymanager/All#Adafruit_NeoPixel

#include <Adafruit_MCP23008.h> // Click here to get the library: http://librarymanager/All#Adafruit_MCP23008

#pragma endregion

#pragma region Classes

/**
 * @brief Made4Home IO board abstraction class.
 * 
 */
class Made4Home_t
{
    protected:

        /** 
         * @brief Blink timer instance.
         */
        Adafruit_NeoPixel *m_LEDs;

        /**
         * @brief Two Wire Interface with index 0.
         */
        TwoWire *m_TWIOne;

        /**
         * @brief MCP IO chip abstraction instance.
         */
        Adafruit_MCP23008 *m_MCP;

    public:

        /**
         * @brief Setup the IO board.
         * 
         */
        void setup();

        /**
         * @brief Set the color of the LEDs.
         * 
         * @param index Index of the LEDs [0-1]
         * @param r Red color. [0-255]
         * @param g Green color [0-255]
         * @param b Blue color [0-255]
         * @return * void 
         */
        void setLED(int index, int r, int g, int b);

        /**
         * @brief Set the color of the L1 LED.
         * 
         * @param r Red color. [0-255]
         * @param g Green color [0-255]
         * @param b Blue color [0-255]
         * @return * void 
         */
        void setL1(int r, int g, int b);

        /**
         * @brief Set the color of the L2 LED.
         * 
         * @param r Red color. [0-255]
         * @param g Green color [0-255]
         * @param b Blue color [0-255]
         * @return * void 
         */
        void setL2(int r, int g, int b);
        
        /**
         * @brief Read the digital inputs of the IO board.
         * 
         * @param pin Pin number [PIN_IN_1 .. 4]
         * @return int Returns HIGH or LOW.
         */
        int digitalRead(int pin);

        /**
         * @brief Write to the relay outputs of the IO board.
         * 
         * @param pin Pin number [PIN_RELAY_1 .. 4]
         * @param state HIGH or LOW
         */
        void digitalWrite(int pin, int state);
};

#pragma endregion

#pragma region Instance

/**
 * @brief Instance of the IO board abstraction.
 * 
 */
extern Made4Home_t Made4Home;

#pragma endregion

#endif
