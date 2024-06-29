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

#define UPDATE_INTERVAL_MS 5000

#pragma endregion

#pragma region Headers

#include <ETH.h>

#include "made4home.h"

#include "FxTimer.h"

#pragma endregion

#pragma region Variables

/**
 * @brief Ethernet connection state.
 *
 */
static bool EthernetConnected_g = false;

/**
 * @brief Update timer instance.
 */
FxTimer *UpdateTimer_g;

#pragma endregion

#pragma region Prototypes

/**
 * @brief Network event state handler.
 *
 * @param event Event input.
 */
void wifi_event(WiFiEvent_t event);

/**
 * @brief Do HTTP request.
 *
 * @param host Host
 * @param port Port
 */
void do_client(const char *host, uint16_t port);

#pragma endregion

void setup()
{
    // Setup the serial port.
    Serial.begin(DEFAULT_BAUDRATE, SERIAL_8N1);
    while (!Serial)
    {
    }

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

    // Setup the update timer.
    UpdateTimer_g = new FxTimer();
    UpdateTimer_g->setExpirationTime(UPDATE_INTERVAL_MS);
    UpdateTimer_g->updateLastTime();
}

void loop()
{
    UpdateTimer_g->update();
    if (UpdateTimer_g->expired())
    {
        UpdateTimer_g->updateLastTime();
        UpdateTimer_g->clear();

        if (EthernetConnected_g)
        {
            do_client("google.com", 80);
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
        // set eth hostname here
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

/**
 * @brief Do HTTP request.
 *
 * @param host Host
 * @param port Port
 */
void do_client(const char *host, uint16_t port)
{
    Serial.print("\nconnecting to ");
    Serial.println(host);

    WiFiClient client;
    if (!client.connect(host, port))
    {
        Serial.println("connection failed");
        return;
    }
    client.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
    while (client.connected() && !client.available())
        ;
    while (client.available())
    {
        Serial.write(client.read());
    }

    Serial.println("closing connection\n");
    client.stop();
}

#pragma endregion
