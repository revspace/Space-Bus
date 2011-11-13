include ../../Makefile.inc

CFLAGS	+= -I../../lib/ -I../../lib/tiny485

all : t485-test.hex sblp-send-test.hex sblp-recv-test.hex

clean :
	rm -f *.hex *.o *.elf

t485-test.o:	t485-test.c ../../lib/interop.h
	$(CC) $(CFLAGS) -c -o $@ $<

sblp-recv-test.o:	sblp-recv-test.c ../../lib/interop.h
	$(CC) $(CFLAGS) -c -o $@ $<

sblp-send-test.o:	sblp-send-test.c ../../lib/interop.h
	$(CC) $(CFLAGS) -c -o $@ $<


t485-test.elf:	t485-test.o ../../lib/tiny485/tiny485.o
	$(CC) $(CFLAGS) -o t485-test.elf t485-test.o ../../lib/tiny485/tiny485.o

sblp-recv-test.elf:	sblp-recv-test.o ../../lib/tiny485/tiny485.o ../../lib/sblp/sblp.o
	$(CC) $(CFLAGS) -o sblp-recv-test.elf sblp-recv-test.o ../../lib/tiny485/tiny485.o ../../lib/sblp/sblp.o

sblp-send-test.elf:	sblp-send-test.o ../../lib/tiny485/tiny485.o ../../lib/sblp/sblp.o
	$(CC) $(CFLAGS) -o sblp-send-test.elf sblp-send-test.o ../../lib/tiny485/tiny485.o ../../lib/sblp/sblp.o


%.hex:	%.elf
	size $<
	avr-objcopy -j .text -j .data -O ihex $< $@
