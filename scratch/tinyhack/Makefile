CC	= avr-gcc
OBJCOPY	= avr-objcopy
AVRDUDE	= avrdude

CFLAGS	= -O3 --std=c99 -g -mmcu=attiny85 -Wall -Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000

all : testdevice.hex

testdevice.hex: testdevice.elf
	$(OBJCOPY) -j .text -O ihex testdevice.elf testdevice.hex

testdevice.elf: testdevice.c
	$(CC) $(CFLAGS) -o testdevice.elf testdevice.c sb.c

program: testdevice.hex
	$(AVRDUDE) -p t85 -c usbtiny -P /dev/ttyUSB0 -e -u -U flash:w:testdevice.hex

clean:
	rm -f testdevice.elf
	rm -f testdevice.hex
