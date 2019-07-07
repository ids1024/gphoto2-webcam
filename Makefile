all: main

main: main.c
	gcc -o $@ $^ -lgphoto2 -lturbojpeg

clean:
	rm -f main

.PHONY: all clean
