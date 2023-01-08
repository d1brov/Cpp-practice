ecopy: main.o lib_effective_copy.a
	gcc -o ecopy main.o lib_effective_copy.a

# making static library
lib_effective_copy.a: effective_copy.o
	ar cr lib_effective_copy.a effective_copy.o

main.o: main.c effective_copy.h
	gcc -c main.c

effective_copy.o: effective_copy.c effective_copy.h
	gcc -c effective_copy.c

clean:
	rm *.o *.a ecopy