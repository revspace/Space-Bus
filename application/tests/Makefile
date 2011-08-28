CC	:= avr-gcc
CFLAGS	:= -O3 -Wall -Wextra -mmcu=attiny85 -I../../lib/tiny485 -I../../lib/sblp

all : t485-test.hex

clean :
	rm -f t485-test.hex
	rm -f t485-test.hex

%.o:	%.c ../../lib/tiny485/tiny485.h
	$(CC) $(CFLAGS) -c -o $@ $<

%.elf:	%.o ../../lib/tiny485/tiny485.o
	$(CC) $(CFLAGS) -o $@ $< ../../lib/tiny485/tiny485.o

%.hex:	%.elf
	avr-objcopy -j .text -j .data -O ihex $< $@
