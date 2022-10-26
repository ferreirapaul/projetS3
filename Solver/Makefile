solver: main.o solver.o
	gcc -o solver main.o solver.o -g -fsanitize=address

main.o: main.c
	gcc -c -o main.o main.c -fsanitize=address

solver.o: solver.c
	gcc -c -o solver.o solver.c -fsanitize=address

clean:
	rm -f solver
	rm -f *.o
