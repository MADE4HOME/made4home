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

#define UPDATE_INTERVAL 1000

// #define SECURE_MQTT

#pragma endregion

#pragma region Headers

#include <WiFi.h>

#include "made4home.h"

#include <PubSubClient.h>

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

/**
 * @brief MQTT server domain.
 * 
 */
const char *ClientID_g = "home.iot.loc";

/**
 * @brief MQTT server port.
 * 
 */
const int ServerPort_g = 1883;

#if defined(SECURE_MQTT)

/**
 * @brief Replace with your network credentials.
 * 
 */
const char *ClientName_g = "emqx";

/**
 * @brief Replace with your network credentials.
 * 
 */
const char *ClientPass_g = "public";

#endif

/**
 * @brief Replace with your network credentials.
 * 
 */
const char *GreetingsTopic_g = "made4home/greetings";

/**
 * @brief Chanel 1 relay.
 * 
 */
const char *Output1Topic_g = "made4home/output/1";

/**
 * @brief Chanel 2 relay.
 * 
 */
const char *Output2Topic_g = "made4home/output/2";

/**
 * @brief Chanel 3 relay.
 * 
 */
const char *Output3Topic_g = "made4home/output/3";

/**
 * @brief Chanel 4 relay.
 * 
 */
const char *Output4Topic_g = "made4home/output/4";

/**
 * @brief Replace with your network credentials.
 * 
 */
const char *InputsTopic_g = "made4home/inputs";

/**
 * @brief WiFi client.
 * 
 */
WiFiClient WiFiClient_g;

/**
 * @brief MQTT client.
 * 
 */
PubSubClient *MQTTClient_g;

/**
 * @brief Opto inputs message state.
 * 
 */
String OptoInputsMessage;

#pragma endregion

#pragma region Prototypes

/**
 * @brief MQTT reconnect to the server.
 * 
 */
void mqtt_reconnect();

/**
 * @brief MQTT message callback function.
 * 
 * @param topic Topic of the message.
 * @param payload Payload of the message.
 * @param length Length of the message.
 */
void mqtt_msg_cb(char *topic, byte *payload, unsigned int length);

#pragma endregion

void setup()
{
    // Setup the serial port.
    Serial.begin(115200, SERIAL_8N1);

    // MQTT client.
    MQTTClient_g = new PubSubClient(WiFiClient_g);

  	// Setup the update timer.
	UpdateTimer_g = new FxTimer();
	UpdateTimer_g->setExpirationTime(UPDATE_INTERVAL);
	UpdateTimer_g->updateLastTime();

    // Connect to Wi-Fi network with SSID and password.
    Serial.print("Connecting to ");
    Serial.println(SSID_g);
    WiFi.begin(SSID_g, PASS_g);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    // Print local IP address and start web server.
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Setup the IO board.
    Made4Home.setup();
}

void loop()
{
    if (!MQTTClient_g->connected())
    {
        mqtt_reconnect();
    }
    else
    {
        MQTTClient_g->loop();
    }

    UpdateTimer_g->update();
    if(UpdateTimer_g->expired())
    {
        UpdateTimer_g->updateLastTime();
        UpdateTimer_g->clear();

        if (!MQTTClient_g->connected())
        {
            return;
        }

        // Time controlled process.
        // TODO: Read all inputs and publish it.
        OptoInputsMessage = "[";
        OptoInputsMessage += String(Made4Home.digitalRead(PIN_IN_1) == 0) + ", ";
        OptoInputsMessage += String(Made4Home.digitalRead(PIN_IN_2) == 0) + ", ";
        OptoInputsMessage += String(Made4Home.digitalRead(PIN_IN_3) == 0) + ", ";
        OptoInputsMessage += String(Made4Home.digitalRead(PIN_IN_4) == 0);
        OptoInputsMessage += "]";
        MQTTClient_g->publish(InputsTopic_g, OptoInputsMessage.c_str());
        Serial.println(OptoInputsMessage);
    }
}

#pragma region Functions

/**
 * @brief MQTT reconnect to the server.
 * 
 */
void mqtt_reconnect()
{
    // Connecting to a MQTT broker.
    MQTTClient_g->setServer(ClientID_g, ServerPort_g);
    MQTTClient_g->setCallback(mqtt_msg_cb);

    String client_id = "MADE4HOME-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The MQTT client %s connects to the public MQTT broker\n", client_id.c_str());
    
    // Loop until we're reconnected
    while (!MQTTClient_g->connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
#if defined(SECURE_MQTT)
        if (MQTTClient_g->connect(client_id.c_str(), ClientName_g, ClientPass_g))
#else
        if (MQTTClient_g->connect(client_id.c_str()))
#endif    
        {
            Serial.println("connected");

            // Publish and subscribe
            MQTTClient_g->publish(GreetingsTopic_g, "Hi, I'm MADE4HOME ^^");

            // Subscribe
            MQTTClient_g->subscribe(Output1Topic_g);
            MQTTClient_g->subscribe(Output2Topic_g);
            MQTTClient_g->subscribe(Output3Topic_g);
            MQTTClient_g->subscribe(Output4Topic_g);
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(MQTTClient_g->state());
            delay(2000);
        }
    }
}

/**
 * @brief MQTT message callback function.
 * 
 * @param topic Topic of the message.
 * @param payload Payload of the message.
 * @param length Length of the message.
 */
void mqtt_msg_cb(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp = "";

    // Stringify the payload.    
    for (unsigned int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        messageTemp += (char)payload[i];
    }

    // Print payload.
    Serial.println();

    // Check chanel 1.
    if (String(topic) == Output1Topic_g)
    {
        Serial.print("Changing output 1 to ");
        if(messageTemp == STATE_ON)
        {
            Serial.println(STATE_ON);
            Made4Home.digitalWrite(PIN_RELAY_1, HIGH);
        }
        else if(messageTemp == STATE_OFF)
        {
            Serial.println(STATE_OFF);
            Made4Home.digitalWrite(PIN_RELAY_1, LOW);
        }
    }

    // Check chanel 2.
    else if (String(topic) == Output2Topic_g)
    {
        Serial.print("Changing output 2 to ");
        if(messageTemp == STATE_ON)
        {
            Serial.println(STATE_ON);
            Made4Home.digitalWrite(PIN_RELAY_2, HIGH);
        }
        else if(messageTemp == STATE_OFF)
        {
            Serial.println(STATE_OFF);
            Made4Home.digitalWrite(PIN_RELAY_2, LOW);
        }
    }

    // Check chanel 3.
    else if (String(topic) == Output3Topic_g)
    {
        Serial.print("Changing output 3 to ");
        if(messageTemp == STATE_ON)
        {
            Serial.println(STATE_ON);
            Made4Home.digitalWrite(PIN_RELAY_3, HIGH);
        }
        else if(messageTemp == STATE_OFF)
        {
            Serial.println(STATE_OFF);
            Made4Home.digitalWrite(PIN_RELAY_3, LOW);
        }
    }

    // Check chanel 4.
    else if (String(topic) == Output4Topic_g)
    {
        Serial.print("Changing output 4 to ");
        if(messageTemp == STATE_ON)
        {
            Serial.println(STATE_ON);
            Made4Home.digitalWrite(PIN_RELAY_4, HIGH);
        }
        else if(messageTemp == STATE_OFF)
        {
            Serial.println(STATE_OFF);
            Made4Home.digitalWrite(PIN_RELAY_4, LOW);
        }
    }
}

#pragma endregion
