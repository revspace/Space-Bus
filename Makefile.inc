# target architecture for the AVR portions of the code
# AVRARCH	:= attiny85
AVRARCH	:= attiny44

CC      := avr-gcc
CFLAGS  := -O3 -g -Wall -Wextra -pedantic --std=c99 -mmcu=${AVRARCH}
