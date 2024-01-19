CFLAGS=-Wall -Wextra -Werror -pedantic

all: cacher

cacher: cacher.o
	clang $(CFLAGS) -o cacher cacher.c


cacher.o: cacher.c
	clang $(CFLAGS) -c cacher.c

clean:
	rm -f *.o cacher
