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
      m_MCP->pinMode(PIN_RELAY_1, OUTPUT);
      m_MCP->pinMode(PIN_RELAY_2, OUTPUT);
      m_MCP->pinMode(PIN_RELAY_3, OUTPUT);
      m_MCP->pinMode(PIN_RELAY_4, OUTPUT);
      m_MCP->pinMode(PIN_IN_1, INPUT);
      m_MCP->pinMode(PIN_IN_2, INPUT);
      m_MCP->pinMode(PIN_IN_3, INPUT);
      m_MCP->pinMode(PIN_IN_4, INPUT);
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
    if (pin < PIN_IN_4 || pin > PIN_IN_1)
    {
        return LOW;
    }

    return m_MCP->digitalRead(pin);
}

/**
 * @brief Write to the relay outputs of the IO board.
 * 
 * @param pin Pin number [PIN_RELAY_1 .. 4]
 * @param state HIGH or LOW
 */
void Made4Home_t::digitalWrite(int pin, int state)
{
    if (pin < PIN_RELAY_4 || pin > PIN_RELAY_1)
    {
        return;
    }

    m_MCP->digitalWrite(pin, state);
}

/**
 * @brief Instance of the IO board abstraction.
 * 
 */
Made4Home_t Made4Home;
