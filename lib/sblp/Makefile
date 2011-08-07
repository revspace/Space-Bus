CC	:= avr-gcc
CFLAGS	:= -Os -Wall -Wextra

all : sblp.o

clean :
	rm -f sblp.o

sblp.o : sblp.c sblp.h
	$(CC) $(CFLAGS) -c -o sblp.o sblp.c
