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

#define MB_TIMEOUT_MS 500

#define MB_REG_START 0

#define MB_REG_COUNT 8

#pragma endregion

#pragma region Headers

#include "made4home.h"
#include "FxTimer.h"

#include "ModbusClientRTU.h"
#include "Logging.h"

#pragma endregion

#pragma region Variables

/**
 * @brief Request time.
 * 
 */
uint32_t RequestTime_g;

/**
 * @brief The RS485 module has no half-duplex, so the parameter with the DE/RE pin is required!
 * 
 */
ModbusClientRTU *ModbusClient_g;

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

    // Setup the board IO.
    Made4Home.setup();
   
    // Set up Serial2 connected to Modbus RTU
    RTUutils::prepareHardwareSerial(Serial2);
    Serial2.begin(MB_BAUDRATE, SERIAL_8N1, PIN_RS485_RX, PIN_RS485_TX);

    // Set up ModbusRTU client.
    ModbusClient_g = new ModbusClientRTU(PIN_RS485_EN);

    // - provide onData handler function
    ModbusClient_g->onResponseHandler(&handleResponse);
    // - provide onError handler function
    ModbusClient_g->onErrorHandler(&handleError);
    // Set message timeout to 500ms
    ModbusClient_g->setTimeout(MB_TIMEOUT_MS);
    // Start ModbusRTU background task
    ModbusClient_g->begin(Serial2);

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
        
        // Issue the request
        Error ErrorL = ModbusClient_g->addRequest((uint32_t)millis(), MB_SLAVE_ID, READ_DISCR_INPUT, MB_REG_START, MB_REG_COUNT);
        if (ErrorL!=SUCCESS)
        {
            ModbusError e(ErrorL);
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        }
        
        for (int index = 0; index < PINS_INPUTS_COUNT; index++)
        {
            // Issue the request
            Error ErrorL1 = ModbusClient_g->addRequest(
                (uint32_t)millis(),
                MB_SLAVE_ID,
                WRITE_COIL,
                index,
                Made4Home.digitalRead(index) ? 0xFF00 : 0x0000);
            
            if (ErrorL1!=SUCCESS)
            {
              ModbusError e(ErrorL1);
              LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
            }              
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
void handleResponse(ModbusMessage response, uint32_t token) 
{
    static uint8_t FunctionCodeL = 0;
    static uint8_t InputsL = 0;

    // Get FC.
    FunctionCodeL = response.getFunctionCode();

    // If it is READ_DISCR_INPUT
    if (FunctionCodeL == READ_DISCR_INPUT)
    {
        InputsL = response[3];     
        Made4Home.digitalWrite(0, (InputsL & 0x01) ? 1 : 0);
        Made4Home.digitalWrite(1, (InputsL & 0x02) ? 1 : 0);
        Made4Home.digitalWrite(2, (InputsL & 0x04) ? 1 : 0);
        Made4Home.digitalWrite(3, (InputsL & 0x08) ? 1 : 0);
    }
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
