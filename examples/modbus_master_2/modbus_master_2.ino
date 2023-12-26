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

// Note: this is an example for the "EASTRON SDM630 Modbus-V2" power meter!

#pragma region Definitions

#define UPDATE_INTERVAL 1000

#define MB_BAUDRATE 9600

#define MB_SLAVE_ID 2

#define MB_REG_START 0

#define MB_REG_COUNT 8

#pragma endregion

#pragma region Headers

#include "made4home.h"

#include "FxTimer.h"

#include <ModbusMaster.h>

#pragma endregion

#pragma region Variables

ModbusMaster ModbusMaster_g;

/** 
 * @brief Update timer instance.
 */
FxTimer *UpdateTimer_g;

bool CoilState_g = true;

/**
 * @brief Data ready flag.
 * 
 */
bool DataReadyFlag_g = false;

#pragma endregion

#pragma region Prototypes

#pragma endregion

void setup()
{
    // Init Serial monitor
    Serial.begin(DEFAULT_BAUDRATE);
    while (!Serial) {}

    pinMode(PIN_RS485_EN, OUTPUT);
    // Init in receive mode
    digitalWrite(PIN_RS485_EN, 0);

    // Modbus slave ID 2
    Serial2.begin(MB_BAUDRATE, SERIAL_8N1, PIN_RS485_RX, PIN_RS485_TX);
    ModbusMaster_g.begin(MB_SLAVE_ID, Serial2);
    // Callbacks allow us to configure the RS485 transceiver correctly
    ModbusMaster_g.preTransmission(preTransmission);
    ModbusMaster_g.postTransmission(postTransmission);

    // Setup the update timer.
	  UpdateTimer_g = new FxTimer();
	  UpdateTimer_g->setExpirationTime(UPDATE_INTERVAL);
	  UpdateTimer_g->updateLastTime();

    Made4Home.setup();
    Serial.println("__ OK __");
}

void loop()
{
    
    uint8_t result;
    uint16_t data[6];

    
    UpdateTimer_g->update();
    if(UpdateTimer_g->expired())
    {
        UpdateTimer_g->updateLastTime();
        UpdateTimer_g->clear();

        // Yes.
        DataReadyFlag_g = false;


        // Toggle the coil at address 0x0002 (Manual Load Control)
        // result = ModbusMaster_g.writeSingleCoil(0x00, CoilState_g);
        // CoilState_g = !CoilState_g;
        // if (result == ModbusMaster_g.ku8MBSuccess)
        // {

        // }
        // Read 16 registers starting at 0x3100)
        result = ModbusMaster_g.readDiscreteInputs(MB_REG_START, MB_REG_COUNT);
        Serial.print("Result: ");
        Serial.println(result);
            // We do. Print out the data
            Serial.printf("Requested at %8.3fs:\n", millis() / 1000.0);
            for (uint8_t i = 0; i < MB_REG_COUNT; ++i)
            {
                Serial.printf("     %04X: %i\n", i + MB_REG_START, ModbusMaster_g.getResponseBuffer(i));
            }
            Serial.printf("\r\n\r\n");
        if (result == ModbusMaster_g.ku8MBSuccess)
        {
            // We do. Print out the data
            Serial.printf("Requested at %8.3fs:\n", millis() / 1000.0);
            for (uint8_t i = 0; i < MB_REG_COUNT; ++i)
            {
                Serial.printf("     %04X: %i\n", i + MB_REG_START, ModbusMaster_g.getResponseBuffer(i));
            }
            Serial.printf("\r\n\r\n");
        }
        else
        {
          
        }
    }
    else
    {
        // No, but we may have another response
        if (DataReadyFlag_g)
        {
            DataReadyFlag_g = false;
        }
    }
}

#pragma region Functions

void preTransmission()
{
    digitalWrite(PIN_RS485_EN, 1);
}

void postTransmission()
{
    digitalWrite(PIN_RS485_EN, 0);
}

#pragma endregion
