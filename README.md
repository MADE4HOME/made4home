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

 - [Blink builtin LEDs](https://github.com/MADE4HOME/made4home/blob/develop/examples/blink_leds/blink_leds.ino)
 - [Relay outputs](https://github.com/MADE4HOME/made4home/blob/develop/examples/relays_inputs/relays_inputs.ino)
 - [Opto inputs](https://github.com/MADE4HOME/made4home/blob/develop/examples/relays_inputs/relays_inputs.ino)
 - [ETH HTTP client](https://github.com/MADE4HOME/made4home/blob/develop/examples/ethernet_web_client/ethernet_web_client.ino)
 - [ETH HTTP server](https://github.com/MADE4HOME/made4home/blob/develop/examples/ethernet_web_server/ethernet_web_server.ino)
 - [ETH MQTT client]()
 - [WiFi HTTP client](https://github.com/MADE4HOME/made4home/blob/develop/examples/wifi_web_client/wifi_web_client.ino)
 - [WiFi HTTP server](https://github.com/MADE4HOME/made4home/blob/develop/examples/wifi_web_server/wifi_web_server.ino)
 - [WiFi MQTT client]()
 - [RS485 MODBUS Master]()
 - [RS485 MODBUS Slave]()

# Contributing

If you'd like to contribute to this project, please follow these steps:

1. Fork the repository on GitHub.
2. Clone your forked repository to your local machine.
3. Create a new branch for your changes: `git checkout -b my-new-feature`.
4. Make your modifications and write tests if applicable.
5. Commit your changes: `git commit -am 'Add some feature'`.
6. Push the branch to your forked repository: `git push origin my-new-feature`.
7. Create a pull request on the main repository.

We appreciate your contributions!

# License

This project is licensed under the MIT License. See the [MIT](https://www.mit.edu/~amini/LICENSE.md) file for more details.

