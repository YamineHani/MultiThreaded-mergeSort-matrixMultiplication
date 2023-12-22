all: 7609-matrix 7609-sort

7609-matrix: 7609-matrix.c
	gcc -pthread 7609-matrix.c -o 7609-matrix

7609-sort: 7609-sort.c 
	gcc -pthread 7609-sort.c -o 7609-sort

clean:
	rm -f 7609-matrix 7609-sort
