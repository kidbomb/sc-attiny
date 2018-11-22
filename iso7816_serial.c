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

// misc routines
void init_printf(void);
int iso7816_serial_putc(char c, FILE *file);

// must be volatile (change and test in main and ISR)
volatile uint8_t tx_buzy = 0;
volatile uint8_t bit_index;
volatile uint8_t _tx_buffer; 
volatile uint8_t parity = 0; 

/*** ISR ***/

// compare match interrupt service for OCR0A
// call every 93us
ISR(TIM0_COMPA_vect) { 
  // software UART
  // send data
  if (tx_buzy) {
    if (bit_index == 0) {
      sbi(DDRB,  PB4); // PB4 as output
      // start bit (= 0)
      cbi(PORTB, PB4);
    } else if (bit_index <=8) {
      // LSB to MSB
      if (_tx_buffer & 1) {
        sbi(PORTB, PB4);
        parity++;
      } else {
        cbi(PORTB, PB4);
      }
      _tx_buffer >>= 1;        
    } else if (bit_index == 9) {
      //parity
      if(parity % 2 == 1) {
        sbi(PORTB, PB4);
      } else {
        cbi(PORTB, PB4);
      }
    } else if (bit_index == 10) {
      // stop bit (= 1)
      sbi(PORTB, PB4);
    } else if (bit_index >= 11) {
      //guard time
      cbi(DDRB,  PB4); // PB4 as input
      sbi(PORTB, PB4);
      tx_buzy = 0;
    }
    bit_index++;
  }
}

/*** UART routines ***/
// send serial data to ISO7816 port, block until buzy
void iso7816_serial_write(uint8_t tx_byte) {
  while(tx_buzy);
  bit_index  = 0;
  parity = 0;
  _tx_buffer = tx_byte;
  tx_buzy = 1;
}
#ifdef USE_PRINTF
/*** connect software UART to stdio.h ***/
void init_printf() {
  fdevopen(&iso7816_serial_putc, 0);
}

int iso7816_serial_putc(char c, FILE *file) {
  iso7816_serial_write(c);
  return c;
}
#else
void iso7816_serial_print(const char *str) {
  uint8_t i;
  for (i = 0; str[i] != 0; i++) {
    iso7816_serial_write(str[i]);
  }
}
#endif

/*** main routines ***/

void iso7816_serial_setup(void) {
  // Software UART IO
  cbi(DDRB,  PB4); // PB4 as input
  sbi(PORTB, PB4); // serial idle level is '1'
  /* interrup setup */
  // call ISR(TIM0_COMPA_vect) every 93us
  // set CTC mode : clear timer on comapre match
  // -> reset TCNTO (timer 0) when TCNTO == OCR0A
  sbi(TCCR0A, WGM01);
  // prescaler : clk/8 (1 tic = 1us for 8MHz clock)
  sbi(TCCR0B, CS01);
  // compare register A at 93 us
  OCR0A = 93;
  // interrupt on compare match OCROA == TCNT0
  sbi(TIMSK, OCIE0A);
  // Enable global interrupts
  sei();
  #ifdef USE_PRINTF
  // init stdout = serial
  init_printf();
  #endif
}