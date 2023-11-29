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

#define UPDATE_INTERVAL 5000

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
const char* SSID_g = DEFAULT_SSID;

/**
 * @brief Replace with your network credentials.
 * 
 */
const char* PASS_g = DEFAULT_PASS;

const char *ClientID_g = "broker.emqx.io";
const int ServerPort_g = 1883;
const char *ClientName_g = "emqx";
const char *ClientPass_g = "public";

const char *GreetingsTopic_g = "made4home/greetings";
const char *OutputsTopic_g = "made4home/outputs";
const char *InputsTopic_g = "made4home/inputs";

WiFiClient WiFiClient_g;

PubSubClient *MQTTClient_g;

#pragma endregion

#pragma region Prototypes

#pragma endregion

void setup()
{
    // Setup the serial port.
    Serial.begin(115200, SERIAL_8N1);

    // MQTT client.
    MQTTClient_g = new PubSubClient(WiFiClient_g);

  	// Setup the blink timer.
	UpdateTimer_g = new FxTimer();
	UpdateTimer_g->setExpirationTime(UPDATE_INTERVAL);
	UpdateTimer_g->updateLastTime();

    // Connect to Wi-Fi network with SSID and PASS_g
    Serial.print("Connecting to ");
    Serial.println(SSID_g);
    WiFi.begin(SSID_g, PASS_g);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    //connecting to a MQTT broker
    MQTTClient_g->setServer(ClientID_g, ServerPort_g);
    MQTTClient_g->setCallback(mqtt_msg_cb);
    while (!MQTTClient_g->connected()) {
        String client_id = "esp32-MQTTClient_g-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The MQTTClient_g %s connects to the public MQTT broker\n", client_id.c_str());
        if (MQTTClient_g->connect(client_id.c_str(), ClientName_g, ClientPass_g)) {
            Serial.println("Public EMQX MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(MQTTClient_g->state());
            delay(2000);
        }
    }

    // Publish and subscribe
    MQTTClient_g->publish(GreetingsTopic_g, "Hi, I'm MADE4HOME ^^");
    MQTTClient_g->subscribe(OutputsTopic_g);

    // Setup the IO board.
    Made4Home.setup();
}

void loop()
{
    UpdateTimer_g->update();
    if(UpdateTimer_g->expired())
    {
        UpdateTimer_g->updateLastTime();
        UpdateTimer_g->clear();

        // Time controlled process.
        // TODO: Read all inputs and publish it.
        MQTTClient_g->publish(InputsTopic_g, "Inputs message here");
    }

    MQTTClient_g->loop();
}

#pragma region Functions

void mqtt_msg_cb(char *topic, byte *payload, unsigned int length)
{
    // TODO: Parse message and control the relays.
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

#pragma endregion
