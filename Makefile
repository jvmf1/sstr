CC=cc
NAME=sstr
LIBDESTDIR=/usr/local/lib
INCLUDEDESTDIR=/usr/local/include

all: lib$(NAME).a

$(NAME).o: $(NAME).c $(NAME).h
	$(CC) $(CFLAGS) $< -c -o $@

lib$(NAME).a: $(NAME).o
	ar rcs lib$(NAME).a $<

install: lib$(NAME).a
	mkdir -p $(LIBDESTDIR)
	mkdir -p $(INCLUDEDESTDIR)
	cp -f lib$(NAME).a $(LIBDESTDIR)
	cp -f $(NAME).h $(INCLUDEDESTDIR)

uninstall:
	rm -f $(LIBDESTDIR)/lib$(NAME).a
	rm -f $(INCLUDEDESTDIR)/$(NAME).h

clean:
	rm *.o *.a
