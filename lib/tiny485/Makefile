CC	:= avr-gcc
CFLAGS	:= -Os -Wall -Wextra

all : tiny485_attiny85.o tiny485_attiny4313.o

clean :
	rm -f tiny485_*.o

tiny485_attiny85.o : tiny485.c tiny485.h tiny485_pin.h
	$(CC) $(CFLAGS) -mmcu=attiny85 -c -o tiny485.o tiny485.c

tiny485_attiny4313.o : tiny485.c tiny485.h tiny485_pin.h
	$(CC) $(CFLAGS) -mmcu=attiny4313 -c -o tiny485.o tiny485.c


