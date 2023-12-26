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

#define UPDATE_INTERVAL 1 // [ms]

#define MB_BAUDRATE 9600

#define MB_TIMEOUT 2000

#define MB_SLAVE_ID 1

#pragma endregion

#pragma region Headers

#include "HardwareSerial.h"

#include "ModbusServerRTU.h"

#include "FxTimer.h"

#include "made4home.h"

#pragma endregion

#pragma region Variables

/**
 * @brief Create a ModbusRTU server instance listening with 2000ms timeout.
 * 
 */
ModbusServerRTU *MBserver_g;

/** 
 * @brief Update timer instance.
 */
FxTimer *UpdateTimer_g;

uint8_t InputsState_g = 0;

#pragma endregion

#pragma region Prootypes

/**
 * @brief FC01: worker do serve Modbus function code 0x03 (READ_HOLD_REGISTER).
 * 
 * @param request 
 * @return ModbusMessage 
 */
ModbusMessage FC01(ModbusMessage request);

/**
 * @brief FC03: worker do serve Modbus function code 0x03 (READ_HOLD_REGISTER).
 * 
 * @param request 
 * @return ModbusMessage 
 */
ModbusMessage FC02(ModbusMessage request);

/**
 * @brief FC03: worker do serve Modbus function code 0x03 (READ_HOLD_REGISTER).
 * 
 * @param request 
 * @return ModbusMessage 
 */
ModbusMessage FC03(ModbusMessage request);

#pragma endregion

void setup()
{
    // Init Serial monitor
    Serial.begin(DEFAULT_BAUDRATE);
    while (!Serial) {}
    Serial.println("__ OK __");

    Made4Home.setup();

    MBserver_g = new ModbusServerRTU(MB_TIMEOUT, PIN_RS485_EN);

    // Set up Serial2 connected to Modbus RTU
    RTUutils::prepareHardwareSerial(Serial2);
    Serial2.begin(MB_BAUDRATE, SERIAL_8N1, PIN_RS485_RX, PIN_RS485_TX);

    // Register served function code worker for server 1, FC 0x03
    MBserver_g->registerWorker(MB_SLAVE_ID, READ_COIL, &FC01);
    // Register served function code worker for server 1, FC 0x03
    MBserver_g->registerWorker(MB_SLAVE_ID, READ_DISCR_INPUT, &FC02);
    // Register served function code worker for server 1, FC 0x03
    MBserver_g->registerWorker(MB_SLAVE_ID, READ_HOLD_REGISTER, &FC03);

    // Start ModbusRTU background task
    MBserver_g->begin(Serial2);

  	// Setup the update timer.
	  UpdateTimer_g = new FxTimer();
	  UpdateTimer_g->setExpirationTime(UPDATE_INTERVAL);
	  UpdateTimer_g->updateLastTime();
}

void loop()
{
    UpdateTimer_g->update();
    if(UpdateTimer_g->expired())
    {
        UpdateTimer_g->updateLastTime();
        UpdateTimer_g->clear();

        for (uint8_t index = 0; index < PINS_INPUTS; index++)
        {
            InputsState_g = InputsState_g << 1;
            InputsState_g |= Made4Home.digitalRead(PinsInputs[index]);
        }
    }
}

#pragma region Functions

// FC03: worker do serve Modbus function code 0x03 (READ_HOLD_REGISTER)
ModbusMessage FC01(ModbusMessage request)
{
  uint16_t address;           // requested register address
  uint16_t words;             // requested number of registers
  ModbusMessage response;     // response message to be sent back

  // get request values
  request.get(2, address);
  request.get(4, words);

  // Address and words valid? We assume 10 registers here for demo
  if (address && words && (address + words) <= 10) {
    // Looks okay. Set up message with serverID, FC and length of data
    response.add(request.getServerID(), request.getFunctionCode(), (uint8_t)(words * 2));
    // Fill response with requested data
    for (uint16_t i = address; i < address + words; ++i) {
      Serial.print("ADD: ");
      Serial.println(i);
      response.add(i);
    }
    Serial.println("-----------------------");

  } else {
    // No, either address or words are outside the limits. Set up error response.
    // response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
  }
  return response;
}

// FC01: worker do serve Modbus function code 0x03 (READ_HOLD_REGISTER)
ModbusMessage FC02(ModbusMessage request)
{
    uint16_t address;           // requested register address
    uint16_t words;             // requested number of registers
    ModbusMessage response;     // response message to be sent back
    
    // get request values
    request.get(2, address);
    request.get(4, words);
    
    // Address and words valid? We assume 8 registers here for demo.
    if (address && words && (address + words) <= 8)
    {
        // Looks okay. Set up message with serverID, FC and length of data
        response.add(request.getServerID(), request.getFunctionCode(), (uint8_t)(words * 2));
        
        // Fill response with requested data
        response.add(0x00);
        response.add(InputsState_g);
    }
    else
    {
        // No, either address or words are outside the limits. Set up error response.
        // response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
    }

    return response;
}

// FC03: worker do serve Modbus function code 0x03 (READ_HOLD_REGISTER)
ModbusMessage FC03(ModbusMessage request)
{
  uint16_t address;           // requested register address
  uint16_t words;             // requested number of registers
  ModbusMessage response;     // response message to be sent back

  // get request values
  request.get(2, address);
  request.get(4, words);

  // Address and words valid? We assume 10 registers here for demo
  if (address && words && (address + words) <= 10) {
    // Looks okay. Set up message with serverID, FC and length of data
    response.add(request.getServerID(), request.getFunctionCode(), (uint8_t)(words * 2));
    // Fill response with requested data
    for (uint16_t i = address; i < address + words; ++i) {
      Serial.print("ADD: ");
      Serial.println(i);
      response.add(i);
    }
    Serial.println("-----------------------");

  } else {
    // No, either address or words are outside the limits. Set up error response.
    // response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
  }
  return response;
}

#pragma endregion