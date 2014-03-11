main:main.o fun.o
	gcc main.o fun.o -o main
main.o:main.c fun.h
	gcc -c main.c -o main.o
fun.o:fun.c fun.h
	gcc -c fun.c -o fun.o
clean:
	rm -f main *.o
