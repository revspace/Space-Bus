# configure target architecture
AVRARCH	:= attiny4313

OBJDIR	:= obj/${AVRARCH}

CC      := avr-gcc
CFLAGS  := -O3 -Wall -Wextra -mmcu=${AVRARCH} 

