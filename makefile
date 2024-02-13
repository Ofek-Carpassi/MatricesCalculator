mygrop: mainmat.o mymat.o
	gcc -g mainmat.o mymat.o -o mygrop

mainmat.o: mainmat.c mymat.h
	gcc -c -ansi -Wall -pedantic mainmat.c -o mainmat.o
	
mymat.o: mymat.c mymat.h
	gcc -c -ansi -Wall -pedantic mymat.c -o mymat.o