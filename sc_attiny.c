/*
 Original code from https://github.com/sourceperl/tiny.serial/blob/master/serial.c

*/

#define F_CPU 8000000UL
#define USE_PRINTF 
#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "iso7816_serial.h"

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) // clear bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))  // set bit

/* ATTiny85 IO pins

             ___^___
RST        -|PB5 VCC|-
           -|PB3 PB2|-
I/O        -|PB4 PB1|-
           -|GND PB0|-
             -------
*/

/* prototypes */
// main routines

int main(void) {
  iso7816_serial_setup();

  _delay_ms(1);

  //TS
  //0x3B =  Direct Convention
  iso7816_serial_write(0x3b);

  //T0
  //Y(1): b0110, 
  //K (LSB): 15 (historical bytes)
  iso7816_serial_write(0x6F);

  //TB(1) = 0x00
  iso7816_serial_write(0x00);
  //TC(1) = 0x00
  iso7816_serial_write(0x00);

  //historical bytes
  iso7816_serial_write(0x00);
  iso7816_serial_write(0x01);
  iso7816_serial_write(0x02);
  iso7816_serial_write(0x03);
  iso7816_serial_write(0x04);

  iso7816_serial_write(0x05);
  iso7816_serial_write(0x06);
  iso7816_serial_write(0x07);
  iso7816_serial_write(0x08);
  iso7816_serial_write(0x09);

  iso7816_serial_write(0x0A);
  iso7816_serial_write(0x0B);
  iso7816_serial_write(0x0C);
  iso7816_serial_write(0x0D);
  iso7816_serial_write(0x0E);
  
  for(;;) {
  }
};
