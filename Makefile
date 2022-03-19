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
	rm -f lib$(NAME).a $(LIBDESTDIR)
	rm -f $(NAME).h $(INCLUDEDESTDIR)

clean:
	rm *.o *.a
