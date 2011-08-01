all:
	make -C lib
	make -C application

clean:
	make -C lib clean
	make -C application clean

docs:
	doxygen Doxyfile
