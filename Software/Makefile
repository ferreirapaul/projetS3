main: main.o
	gcc main.o `pkg-config --libs gtk+-3.0` `pkg-config --libs sdl2 SDL2_image`-o main -lm

main.o: main.c
	gcc -O3 `pkg-config --cflags sdl2 SDL2_image``pkg-config --cflags gtk+-3.0`-c main.c -o main.o

clean:
	rm -f *.o
	rm -f main
	rm -f *.glade~
