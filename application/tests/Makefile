include ../../Makefile.inc

CFLAGS	+= -I../../lib/ -I../../lib/tiny485

all : t485-test.hex sblp-test.hex

clean :
	rm -f *.hex *.o *.elf

t485-test.o:	t485-test.c ../../lib/interop.h
	$(CC) $(CFLAGS) -c -o $@ $<

sblp-test.o:	sblp-test.c ../../lib/interop.h
	$(CC) $(CFLAGS) -c -o $@ $<


t485-test.elf:	t485-test.o ../../lib/tiny485/tiny485.o
	$(CC) $(CFLAGS) -o t485-test.elf t485-test.o ../../lib/tiny485/tiny485.o

sblp-test.elf:	sblp-test.o ../../lib/tiny485/tiny485.o ../../lib/sblp/sblp.o
	$(CC) $(CFLAGS) -o sblp-test.elf sblp-test.o ../../lib/tiny485/tiny485.o ../../lib/sblp/sblp.o


%.hex:	%.elf
	size $<
	avr-objcopy -j .text -j .data -O ihex $< $@
