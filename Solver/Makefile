solver: main.o solver.o
	gcc -o solver main.o solver.o 

main.o: main.c
	gcc -c -o main.o main.c

solver.o: solver.c
	gcc -c -o solver.o solver.c 

clean:
	rm -f solver
	rm -f *.o
	rm -f *.result
