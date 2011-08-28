CC	:= avr-gcc
AVRARCH := attiny4313
CFLAGS	:= -O3 -Wall -Wextra -mmcu=${AVRARCH} -I../../lib/tiny485 -I../../lib/sblp
TINY485O:= ../../lib/tiny485/tiny485_${AVRARCH}.o

all : t485-test.hex

clean :
	rm -f t485-test.hex
	rm -f t485-test.hex

%.o:	%.c ../../lib/tiny485/tiny485.h
	$(CC) $(CFLAGS) -c -o $@ $<

ifeq ($(AVRARCH),attiny4313)
%.elf:	%.o ../../lib/tiny485/tiny485_attiny4313.o
	$(CC) $(CFLAGS) -o $@ $< ../../lib/tiny485/tiny485_${AVRARCH}.o
else
ifeq ($(AVRARCH),attiny85)
%.elf:	%.o ../../lib/tiny485/tiny485_attiny4313.o
	$(CC) $(CFLAGS) -o $@ $< ../../lib/tiny485/tiny485_${AVRARCH}.o
endif
endif

%.hex:	%.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

# ../../lib/tiny485/tiny485_${AVRARCH}.o

