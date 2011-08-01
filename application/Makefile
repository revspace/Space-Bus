SUBDIRS=elrc

all:
	@for DIR in $(SUBDIRS); do \
	  make -C $$DIR ; \
	done

clean:
	@for DIR in $(SUBDIRS); do \
	  make -C $$DIR clean ; \
	done

