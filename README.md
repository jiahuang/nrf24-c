#nrf24-c

Based off of the implementation done by [Sparkfun](https://www.sparkfun.com/products/8602)

A small, standalone, barebones c library for the nrf24l01+ transceiver. Originally written for use on chips with limited program space such as the ATtiny24, but should work on non-AVR chips as well if the pins are changed.

Currently handles only transmission and receiving.

Default pin setup (ATTiny24)

* SCK PORTA4
* MISO PORTA5
* MOSI PORTA6
* CE PORTA1
* CSN PORTA2