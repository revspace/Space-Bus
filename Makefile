include Makefile.inc

all:
	make -C lib
	make -C application

clean:
	make -C lib clean
	make -C application clean
	rm -Rf doc

doc:
	mkdir -p doc
	doxygen Doxyfile
