Creates a smartcard simulation using an Attiny 85

Connections:
 * VCC to VCC
 * GND to GND
 * RST to RST (PB5)
 * I/O to PB4

 Currently it can only generate an ATR

To build, use the "make build" command. Then, use the "burn.sh" script to write the firmware to your attiny85

TODO:
 * add support to send and receive APDUs