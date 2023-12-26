## Intro

This repository is created to provide an easy to use framework for controlling MADE4HOME module. The library is written in C/C++ in Arduino manner.

## Dependencies

The framework depends on different hardware submodules that are part of the MADE4HOME product.

We will provide links to libraries that we choose. The reasons to choose these libraries are because they are from the same provider and are with the same code quality.

 - [Relay Outputs and Opto Inputs (MCP23008)](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library)
 - [LEDs](https://github.com/adafruit/Adafruit_NeoPixel)
 - [Crypto Module](https://github.com/sparkfun/SparkFun_ATECCX08a_Arduino_Library)
 - [RS485 Interface](https://github.com/eModbus/eModbus)
 - [MQTT Client](https://github.com/knolleary/pubsubclient)

## Examples

The examples cover the most popular communication topics and IOs options that this module provides.

### Blink built-in LEDs

This is the most standard example. It is so simple that it provides only a method to control the LEDs of the board.

 - [Blink builtin LEDs](https://github.com/MADE4HOME/made4home/blob/develop/examples/blink_leds/blink_leds.ino)

### Opto inputs & Relay outputs

The most beautiful way to test both inputs and outputs of the board is to bridge them together. The example shows the most simple way to do this.

 - [Opto inputs & Relay outputs](https://github.com/MADE4HOME/made4home/blob/develop/examples/relays_inputs/relays_inputs.ino)

### Ethernet HTTP Client

One of the oldest, most popular protocols for information exchange between intelligent systems via internet is HTTP. So, this example demonstrates how to call an internet endpoint via this board.

 - [ETH HTTP client](https://github.com/MADE4HOME/made4home/blob/develop/examples/ethernet_web_client/ethernet_web_client.ino)

### Ethernet HTTP Server

Of course, at the other side of the line someone should answer our calls. That is why we provide not only client but a server that enables you to have an endpoint to call and use it to control relays and monitor inputs.

 - [ETH HTTP server](https://github.com/MADE4HOME/made4home/blob/develop/examples/ethernet_web_server/ethernet_web_server.ino)

### Ethernet MQTT client

On the other hand, the most popular protocol in our "IoT" days is not only HTTP but, of course, MQTT (Message Que Telemetry Transport). This protocol serves thousands of devices all over the world. That's why we provide an example for it.

 - [ETH MQTT client](https://github.com/MADE4HOME/made4home/blob/develop/examples/ethernet_mqtt_client/ethernet_mqtt_client.ino)

Of course there is situations that require to have more security than just a user name and password. In this kind of situations we provide on our board a secure element [ATECC508A](https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/DataSheets/20005928A.pdf) cryptographic processor that can store a certificates and keys. We provide a very simple example of how to use it. For further usage we recommend to use the documentation provided by [Microchip](https://www.microchip.com/en-us/product/ATECC508A).

 - [ETH MQTT secure client](https://github.com/MADE4HOME/made4home/blob/develop/examples/ethernet_mqtt_client_secure/ethernet_mqtt_client_secure.ino)

### Wi-Fi

Later in the development of the Internet and Internet of Things, the industry brought to the people wireless internet. That's when it became possible to implement wireless fidelity inside a small cheap chip. So, the next 3 examples are based on Wi-Fi or wireless fidelity interface of the board.

 - [WiFi HTTP client](https://github.com/MADE4HOME/made4home/blob/develop/examples/wifi_web_client/wifi_web_client.ino)
 - [WiFi HTTP server](https://github.com/MADE4HOME/made4home/blob/develop/examples/wifi_web_server/wifi_web_server.ino)
 - [WiFi MQTT client](https://github.com/MADE4HOME/made4home/blob/develop/examples/wifi_mqtt_clients/wifi_mqtt_clients.ino)

Not very different then previous secure example, here we do the same but the transport interface is WiFi.

 - [WiFi MQTT secure client](https://github.com/MADE4HOME/made4home/blob/develop/examples/wifi_mqtt_client_secure/wifi_mqtt_client_secure.ino)

### Modbus

 - [RS485 MODBUS Master]()
 - [RS485 MODBUS Slave]()

# Contributing

If you'd like to contribute to this project, please follow these steps:

1. Fork the repository on GitHub.
2. Clone your forked repository to your local machine.
3. Create a new branch for your changes:
```sh
git checkout -b my-new-feature
```
4. Make your modifications and write tests if applicable.
5. Commit your changes: 
```sh
git commit -m 'Add some feature'
```
6. Push the branch to your forked repository:
```sh
git push origin my-new-feature
```
7. Create a pull request on the main repository.

We appreciate your contributions!

# License

This project is licensed under the MIT License. See the [MIT](https://www.mit.edu/~amini/LICENSE.md) file for more details.
