CC	= avr-gcc
OBJCOPY	= avr-objcopy
AVRDUDE	= avrdude

CFLAGS	= -O3 --std=c99 -g -mmcu=attiny85 -Wall -Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000 -DF_CPU=1000000UL
ADFLAGS	= -p t85 -c usbtiny -P /dev/ttyUSB0 -e -u

all : test.hex

test.hex: test.elf
	$(OBJCOPY) -j .text -O ihex test.elf test.hex

test.elf: test.c sbp.c sbp.h sbp_config.h
	$(CC) $(CFLAGS) -o test.elf test.c sbp.c

program: test.hex
	$(AVRDUDE) -U flash:w:test.hex

clean:
	rm -f test.elf
	rm -f test.hex
