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

#define MB_PORT 502

#define MB_SLAVE_ID 2

#define MB_TIMEOUT_MS 2000

#define MB_INTERVAL_MS 200

#define MB_REG_START 0

#define MB_REG_COUNT 8

#pragma endregion

#pragma region Headers

#include <ETH.h>

#include <WiFi.h>

#include "made4home.h"

// Include the header for the ModbusClient TCP style
#include "ModbusClientTCP.h"
// Include the header for the coils data type
#include "CoilData.h"
#include "Logging.h"

#include "FxTimer.h"

#pragma endregion

#pragma region Variables

/**
 * @brief WiFi client.
 * 
 */
WiFiClient WiFiClient_g;

/**
 * @brief 
 * 
 */
IPAddress TargetIP_g(172, 33, 4, 10);

/**
 * @brief Create a ModbusTCP client instance.
 * 
 */
ModbusClientTCP *ModbusClient_g;

/** 
 * @brief Update timer instance.
 */
FxTimer *UpdateTimer_g;

/**
 * @brief Coil data.
 * 
 */
CoilData CoilData_g(12);

/**
 * @brief Ethernet connection state.
 * 
 */
static bool EthernetConnected_g = false;

#pragma endregion

#pragma region Prototypes

/**
 * @brief Network event state handler.
 * 
 * @param event Event input.
 */
void wifi_event(WiFiEvent_t event);

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
   
    // Attach the network events.
    WiFi.onEvent(wifi_event);

    // Run the Ethernet.
    ETH.begin(
        PIN_ETH_PHY_ADDR,
        PIN_ETH_PHY_POWER,
        PIN_ETH_PHY_MDC,
        PIN_ETH_PHY_MDIO,
        PIN_ETH_PHY_TYPE,
        PIN_ETH_CLK_MODE);

    ModbusClient_g = new ModbusClientTCP(WiFiClient_g);

    // Set up ModbusRTU client.
    // - provide onData handler function
    ModbusClient_g->onResponseHandler(&handleResponse);
    // - provide onError handler function
    ModbusClient_g->onErrorHandler(&handleError);
    // Set message timeout to 500ms
    ModbusClient_g->setTimeout(MB_TIMEOUT_MS, MB_INTERVAL_MS);
    // Start ModbusTCP background task
    ModbusClient_g->begin();
    // Issue a request
    // Set Modbus TCP server address and port number
    // (Fill in your data here!)
    ModbusClient_g->setTarget(TargetIP_g, MB_PORT);

    // Setup the update timer.
    UpdateTimer_g = new FxTimer();
    UpdateTimer_g->setExpirationTime(UPDATE_INTERVAL_MS);
    UpdateTimer_g->updateLastTime();

    // Setup default value of the coils.
    CoilData_g = "000000000000";
}

void loop()
{
    static Error ErrorL;

    UpdateTimer_g->update();
    if(UpdateTimer_g->expired())
    {
        UpdateTimer_g->updateLastTime();
        UpdateTimer_g->clear();

        // Issue the request
        ErrorL = ModbusClient_g->addRequest((uint32_t)millis(), MB_SLAVE_ID, READ_DISCR_INPUT, MB_REG_START, MB_REG_COUNT);
        if (ErrorL!=SUCCESS)
        {
          ModbusError e(ErrorL);
          LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        }

        for (int index = 0; index < PINS_INPUTS_COUNT; index++)
        {
            CoilData_g.set(index, (Made4Home.digitalRead(index) == HIGH));
        }

        // Issue the request
        ErrorL = ModbusClient_g->addRequest(
            (uint32_t)millis(),
            MB_SLAVE_ID,
            WRITE_MULT_COILS,
            0, CoilData_g.coils(), CoilData_g.size(), CoilData_g.data());

        if (ErrorL!=SUCCESS)
        {
          ModbusError e(ErrorL);
          LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
        }
    }
}

#pragma region Functions

/**
 * @brief Network event state handler.
 * 
 * @param event Event input.
 */
void wifi_event(WiFiEvent_t event)
{
    switch (event)
    {
    case ARDUINO_EVENT_ETH_START:
        Serial.println("ETH Started");
        //set eth hostname here
        ETH.setHostname("made4home");
        break;
    case ARDUINO_EVENT_ETH_CONNECTED:
        Serial.println("ETH Connected");
        break;
    case ARDUINO_EVENT_ETH_GOT_IP:
        Serial.print("ETH MAC: ");
        Serial.print(ETH.macAddress());
        Serial.print(", IPv4: ");
        Serial.print(ETH.localIP());
        if (ETH.fullDuplex())
        {
            Serial.print(", FULL_DUPLEX");
        }
        Serial.print(", ");
        Serial.print(ETH.linkSpeed());
        Serial.println("Mbps");
        EthernetConnected_g = true;
        break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
        Serial.println("ETH Disconnected");
        EthernetConnected_g = false;
        break;
    case ARDUINO_EVENT_ETH_STOP:
        Serial.println("ETH Stopped");
        EthernetConnected_g = false;
        break;
    default:
        break;
    }
}

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
        Serial.printf("Inputs: %d", InputsL);
        Serial.println();
      
        Made4Home.digitalWrite(0, (InputsL & 0x01) ? 1 : 0);
        Made4Home.digitalWrite(1, (InputsL & 0x02) ? 1 : 0);
        Made4Home.digitalWrite(2, (InputsL & 0x04) ? 1 : 0);
        Made4Home.digitalWrite(3, (InputsL & 0x08) ? 1 : 0);

        // for(uint16_t index = 0; index < response.size(); index++)
        // {
        //     Serial.printf("%02X: %02X, ", index, response[index]);
        // }
        // Serial.println();
    }

    Serial.println("handleResponse");
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
