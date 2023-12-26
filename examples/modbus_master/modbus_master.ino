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

#define UPDATE_INTERVAL_MS 1000

#define MB_BAUDRATE 9600

#define MB_SLAVE_ID 2

#define MB_REG_START 0

#define MB_REG_COUNT 8

#pragma endregion

#pragma region Headers

#include "made4home.h"

// Include the header for the ModbusClient RTU style
#include "ModbusClientRTU.h"
#include "Logging.h"

#include "FxTimer.h"

#pragma endregion

#pragma region Variables

/**
 * @brief Data ready flag.
 * 
 */
bool DataReadyFlag_g = false;

/**
 * @brief Response values.
 * 
 */
vector<uint8_t> Values_g;

/**
 * @brief Request time.
 * 
 */
uint32_t RequestTime_g;

/**
 * @brief The RS485 module has no halfduplex, so the parameter with the DE/RE pin is required!
 * 
 */
ModbusClientRTU *ModbusClientRTU_g;

/** 
 * @brief Update timer instance.
 */
FxTimer *UpdateTimer_g;

#pragma endregion

#pragma region Prototypes

/**
 * @brief Define an onData handler function to receive the regular responses
 * 
 * @param response Received response message.
 * @param token Request's token.
 */
void handleData(ModbusMessage response, uint32_t token);

/**
 * @brief Define an onError handler function to receive error responses
 * 
 * @param error Error code.
 * @param token User-supplied token to identify the causing request.
 */
void handleError(Error error, uint32_t token);

#pragma endregion

void setup()
{
    // Init Serial monitor
    Serial.begin(DEFAULT_BAUDRATE, SERIAL_8N1);
    while (!Serial) {}

    Made4Home.setup();

    for (int i = 1; i <= MB_REG_COUNT*2; i++)
    {
      Values_g.push_back(i);
    }
 
    Serial.println("__ OK __");
   
    // Set up Serial2 connected to Modbus RTU
    RTUutils::prepareHardwareSerial(Serial2);
    Serial2.begin(MB_BAUDRATE, SERIAL_8N1, PIN_RS485_RX, PIN_RS485_TX);

    ModbusClientRTU_g = new ModbusClientRTU(PIN_RS485_EN);

    // Set up ModbusRTU client.
    // - provide onData handler function
    // ModbusClientRTU_g->onResponseHandler(&handleResponse);
    // - provide onData handler function
    ModbusClientRTU_g->onDataHandler(&handleData);
    // - provide onError handler function
    ModbusClientRTU_g->onErrorHandler(&handleError);
    // Set message timeout to 2000ms
    ModbusClientRTU_g->setTimeout(2000);
    // Start ModbusRTU background task
    ModbusClientRTU_g->begin(Serial2);

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

        // Yes.
        DataReadyFlag_g = false;

        // Issue the request
        // Error ErrorL = ModbusClientRTU_g->addRequest((uint32_t)millis(), MB_SLAVE_ID, READ_COIL, MB_REG_START, MB_REG_COUNT);
        // if (ErrorL!=SUCCESS)
        // {
        //   ModbusError e(ErrorL);
        //   LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        // }

        // Issue the request
        Error ErrorL = ModbusClientRTU_g->addRequest((uint32_t)millis(), MB_SLAVE_ID, READ_DISCR_INPUT, MB_REG_START, MB_REG_COUNT);
        if (ErrorL!=SUCCESS)
        {
          ModbusError e(ErrorL);
          LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        }

        // // Issue the request
        // ErrorL = ModbusClientRTU_g->addRequest((uint32_t)millis(), MB_SLAVE_ID, READ_INPUT_REGISTER, MB_REG_START, MB_REG_COUNT);
        // if (ErrorL!=SUCCESS)
        // {
        //   ModbusError e(ErrorL);
        //   LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        // }

        // // Issue the request
        // ErrorL = ModbusClientRTU_g->addRequest((uint32_t)millis(), MB_SLAVE_ID, READ_HOLD_REGISTER, MB_REG_START, MB_REG_COUNT);
        // if (ErrorL!=SUCCESS)
        // {
        //   ModbusError e(ErrorL);
        //   LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        // }
    }
    else
    {
        // No, but we may have another response
        if (DataReadyFlag_g)
        {
            DataReadyFlag_g = false;

            // We do. Print out the data
            // Serial.printf("Requested at %8.3fs:\n", RequestTime_g / 1000.0);
            // for (uint8_t i = 0; i < MB_REG_COUNT * 2; ++i)
            // {
            //   Serial.printf("   %04X: %i\n", i + MB_REG_START, Values_g[i]);
            // }
            // Serial.printf("\r\n\r\n");
        }
    }
}

#pragma region Functions

/**
 * @brief Define an onData handler function to receive the regular responses
 * 
 * @param response Received response message.
 * @param token Request's token.
 */
void handleData(ModbusMessage response, uint32_t token) 
{
  // // First value is on pos 3, after server ID, function code and length byte
  // uint16_t offs = 0;

  // // The device has values all as IEEE754 float32 in two consecutive registers
  // // Read the requested in a loop
  // for (uint8_t i = 0; i < MB_REG_COUNT; ++i)
  // {
  //   response.get(i, Values_g[i]);
  // }

  for (uint8_t i = 0; i < response.size(); ++i)
  {
    // response.get(i, Values_g[i]);
    // Values_g[i] = response[i];
    Serial.printf("%02X: %02X, ", i, response[i]);
  }

  Serial.println();


  // uint16_t addr, words;
  // response.get(2, addr);
  // response.get(4, words);
  // Serial.print("Addr: ");
  // Serial.print(addr);
  // Serial.print(" Word: ");
  // Serial.println(words);
  Serial.println("handleData");

  // Signal "data is complete"
  RequestTime_g = token;
  DataReadyFlag_g = true;
}

void handleResponse(ModbusMessage response, uint32_t token) 
{
  // // First value is on pos 3, after server ID, function code and length byte
  // uint16_t offs = 0;

  // // The device has values all as IEEE754 float32 in two consecutive registers
  // // Read the requested in a loop
  for (uint8_t i = 0; i < response.size(); ++i)
  {
    // response.get(i, Values_g[i]);
    // Values_g[i] = response[i];
    Serial.printf("%02X: %02X, ", i, response[i]);
  }

  Serial.println();

  
  // uint16_t addr, words;
  // response.get(2, addr);
  // response.get(4, words);
  // Serial.print("Addr: ");
  // Serial.print(addr);
  // Serial.print(" Word: ");
  // Serial.println(words);
  Serial.println("handleResponse");

  // Signal "data is complete"
  RequestTime_g = token;
  DataReadyFlag_g = true;
}

/**
 * @brief Define an onError handler function to receive error responses
 * 
 * @param error Error code.
 * @param token User-supplied token to identify the causing request.
 */
void handleError(Error error, uint32_t token) 
{
    // ModbusError wraps the error code and provides a readable error message for it
    ModbusError ModbusErrorL(error);
    LOG_E("Error response: %02X - %s\n", (int)ModbusErrorL, (const char *)ModbusErrorL);
}

#pragma endregion
