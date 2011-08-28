include ../../Makefile.inc

# CFLAGS	:= -Os -Wall -Wextra

all : tiny485.o

clean :
	rm -f tiny485.o

tiny485.o:	tiny485.c tiny485.h tiny485_pin.h
		$(CC) $(CFLAGS) -c -o tiny485.o tiny485.c



