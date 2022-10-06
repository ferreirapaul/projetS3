main: main.c
	#gcc $( pkg-config --cflags gtk4 ) -o main main.c $( pkg-config --libs gtk4 )
	gcc main.c -o main `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` 

clean:
	rm -f *.o
	rm -f main
	rm -f *.glade~
