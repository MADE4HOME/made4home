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

#include <WiFi.h>

#include "made4home.h"

#include "FxTimer.h"

#include "DefaultCredentials.h"

#pragma endregion

#pragma region Variables

/**
 * @brief Update timer instance.
 */
FxTimer *UpdateTimer_g;

/**
 * @brief Replace with your network credentials.
 *
 */
const char *SSID_g = DEFAULT_SSID;

/**
 * @brief Replace with your network credentials.
 *
 */
const char *PASS_g = DEFAULT_PASS;

#pragma endregion

#pragma region Prototypes

/**
 * @brief Connect to WiFi.
 *
 */
void connect_to_wifi();

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

    // Setup the IO board.
    Made4Home.setup();

    // Connect to Wi-Fi network with SSID and password.
    connect_to_wifi();

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
        do_client("google.com", 80);
    }
}

#pragma region Functions

/**
 * @brief Connect to WiFi.
 *
 */
void connect_to_wifi()
{
    Serial.print("Connecting to ");
    Serial.println(SSID_g);
    WiFi.begin(SSID_g, PASS_g);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.print("Connected to ");
    Serial.println(SSID_g);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
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
