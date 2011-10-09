include ../../Makefile.inc

CFLAGS	+= -I../../lib/ -I../../lib/tiny485

all : t485-test.hex

clean :
	rm -f *.hex *.o *.elf

%.o:	%.c ../../lib/tiny485/tiny485.h
	$(CC) $(CFLAGS) -c -o $@ $<

t485-test-new.elf: t485_test_new.o ../../lib/tiny485/tiny485.o

%.elf:	%.o ../../lib/tiny485/tiny485.o
	$(CC) $(CFLAGS) -o $@ $< ../../lib/tiny485/tiny485.o

%.hex:	%.elf
	size $<
	avr-objcopy -j .text -j .data -O ihex $< $@
