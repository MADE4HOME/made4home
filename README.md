## Intro

This repository is create to give an easy to use framework for controlling MADE4HOME module. The library is written in C/C++ in Arduino manner.

## Dependencies

The framework depend on different hardware submodules that is part of MADE4HOME product.

We will provide a links to libraries that we chose. The reasons to chose this libraries are because it is from the same provider and are with the same code quality.

 - [Relay Outputs and Opto Inputs (MCP23008)](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library)
 - [LEDs](https://github.com/adafruit/Adafruit_NeoPixel)
 - [Crypto Module](https://github.com/sparkfun/SparkFun_ATECCX08a_Arduino_Library)
 - [RS485 Interface](https://github.com/eModbus/eModbus)
 - [MQTT Client](https://github.com/knolleary/pubsubclient)

## Examples

The examples covers the most popular communication topics and IOs options that module provides.

### Blink builtin LEDs

This is tha most standard example. It is simple as much it provide a method to control the LEDs of the board.

 - [Blink builtin LEDs](https://github.com/MADE4HOME/made4home/blob/develop/examples/blink_leds/blink_leds.ino)

### Opto inputs & Relay outputs

The most beautiful way to test the both inputs and outputs of the board is to bridge it together. The example shows the most simple way to do this.

 - [Opto inputs & Relay outputs](https://github.com/MADE4HOME/made4home/blob/develop/examples/relays_inputs/relays_inputs.ino)

### Ethernet HTTP Client

One of the oldest most popular protocols for exchange information between inteligent systems via internet is HTTP. So this example demonstrate how to call to internet endpoint vie this board.

 - [ETH HTTP client](https://github.com/MADE4HOME/made4home/blob/develop/examples/ethernet_web_client/ethernet_web_client.ino)

### Ethernet HTTP Server

Of course, at the other side of the line someone should answer our calls. That is  why we provide not only client but a server. To enable to have endpoint to call and control relays and monitor inputs.

 - [ETH HTTP server](https://github.com/MADE4HOME/made4home/blob/develop/examples/ethernet_web_server/ethernet_web_server.ino)

### Ethernet MQTT client

In the other hand the most popular protocol in our "IoT" days is not only HTTP but of course MQTT. Message Que Telemetry Transport. This protocol serve a thousands of devices all over the world. Thats why we provide an example for it.

 - [ETH MQTT client]()

Later of development of the internet and internet of things industry brings to the people wireless internet. Later years they was able to implement wireless fidelity inside a small cheap chip. So the next 3 examples are based on Wi-Fi or wireless fidelity interface of the board.

### Wi-Fi

 - [WiFi HTTP client](https://github.com/MADE4HOME/made4home/blob/develop/examples/wifi_web_client/wifi_web_client.ino)
 - [WiFi HTTP server](https://github.com/MADE4HOME/made4home/blob/develop/examples/wifi_web_server/wifi_web_server.ino)
 - [WiFi MQTT client](https://github.com/MADE4HOME/made4home/blob/develop/examples/wifi_web_server/wifi_mqtt_clients.ino)

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

