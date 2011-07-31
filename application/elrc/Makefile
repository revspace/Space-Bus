CC	:= avr-gcc
CFLAGS	:= -O3 -Wall -Wextra -mmcu=attiny85 -I../../lib/tiny485

all : elrc.hex test.hex

clean :
	rm -f elrc.o
	rm -f elrc.hex
	rm -f elrc.elf
	rm -f test.o
	rm -f test.hex
	rm -f test.elf

%.o:	%.c ../../lib/tiny485/tiny485.h
	$(CC) $(CFLAGS) -c -o $@ $<

%.elf:	%.o ../../lib/tiny485/tiny485.o
	$(CC) -o $@ $< ../../lib/tiny485/tiny485.o

%.hex:	%.elf
	avr-objcopy -j .text -j .data -O ihex $< $@
