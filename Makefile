all: main

main: main.c
	gcc -o $@ $^ -lgphoto2

clean:
	rm -f main

.PHONY: all clean
