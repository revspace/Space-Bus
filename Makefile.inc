# target architecture for the AVR portions of the code
AVRARCH	:= attiny85

OBJDIR	:= obj/${AVRARCH}

CC      := avr-gcc
CFLAGS  := -O3 -g -Wall -Wextra -pedantic --std=c99 -mmcu=${AVRARCH} 
