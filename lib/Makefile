SUBDIRS=tiny485 sblp

all:
	@for DIR in $(SUBDIRS); do \
	  make -C $$DIR ; \
	done

clean:
	@for DIR in $(SUBDIRS); do \
	  make -C $$DIR clean ; \
	done

