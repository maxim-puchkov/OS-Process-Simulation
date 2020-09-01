all:	main.o Simulation.o Process.o Semaphore.o LIST.o
	gcc -o s main.o Simulation.o Process.o Semaphore.o LIST.o

main.o:	main.c
	gcc -c main.c

Simulation.o: Simulation.c
	gcc -c Simulation.c Simulation.h

Process.o: Process.c
	gcc -c Process.c Process.h

Semaphore.o: Semaphore.c
	gcc -c Semaphore.c Semaphore.h

LIST.o: LIST.c
	gcc -c LIST.c LIST.h

clean:	
	rm -f *.o *.s *.gch s
