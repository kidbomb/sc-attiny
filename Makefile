sc_attiny.o:
	avr-gcc -std=c99 -g -Os -mmcu=attiny85 -c "sc_attiny.c" -o "sc_attiny.o"
iso7816_serial.o:
	avr-gcc -std=c99 -g -Os -mmcu=attiny85 -c "iso7816_serial.c" -o "iso7816_serial.o"
sc_attiny.elf:
	avr-gcc -g -mmcu=attiny85 -o "sc_attiny.elf" "sc_attiny.o" "iso7816_serial.o"
sc_attiny.hex:
	avr-objcopy -j .text -j .data -O ihex "sc_attiny.elf" "sc_attiny.hex"
build: sc_attiny.o iso7816_serial.o sc_attiny.elf sc_attiny.hex
clean:
	rm sc_attiny.o iso7816_serial.o sc_attiny.elf sc_attiny.hex
