all: 
	gcc -g main.c rdp.c monitor.c cpuProcess.c politica.c dataStructures.c -lpthread

run:
	./a.out
	
clean:
	rm *.out

clean_test:
	rm ./test/InvarianteTransicion

clean_all:
	rm *.out
	rm ./test/InvarianteTransicion
