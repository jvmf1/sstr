CC=cc
NAME=sstr
CFLAGS=-ffunction-sections -fdata-sections

all: $(NAME).o

$(NAME).o: $(NAME).c $(NAME).h
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm *.o
