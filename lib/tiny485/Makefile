CC	:= avr-gcc
CFLAGS	:= -O3 -Wall -Wextra -mmcu=attiny85

all : tiny485.o

clean :
	rm -f tiny485.o

tiny485.o : tiny485.c tiny485.h
	$(CC) $(CFLAGS) -c -o tiny485.o tiny485.c
