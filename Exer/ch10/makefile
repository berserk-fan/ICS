4 : main.o csapp.o
	gcc -pthread -o 4 csapp.o main.o
main.o: 4.c ../csapp.h
	gcc -c -o main.o 4.c
csapp.o: ../csapp.c ../csapp.h
	gcc -c ../csapp.c
clean: rm main.o csapp.o
