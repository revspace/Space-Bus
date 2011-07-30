CC	:= avr-gcc
CFLAGS	:= -O3 -mmcu=attiny85 -I../../lib/tiny485

all : elrc.hex

clean :
	rm -f elrc.o
	rm -f elrc.hex
	rm -f elrc.elf

elrc.o : elrc.c ../../lib/tiny485/tiny485.h
	$(CC) $(CFLAGS) -c -o elrc.o elrc.c

elrc.elf:	elrc.o ../../lib/tiny485/tiny485.o
	$(CC) -o $@ elrc.o ../../lib/tiny485/tiny485.o

elrc.hex:	elrc.elf
	avr-objcopy -j .text -j .data -O ihex elrc.elf elrc.hex

