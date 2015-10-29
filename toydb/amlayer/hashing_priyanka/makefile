a.out: hashmap_reader.o ../../pflayer/pflayer.o main.o
	cc hashmap_reader.o ../../pflayer/pflayer.o main.o

hashmap_reader.o: hashmap_reader.c ../pf.h
	cc -c hashmap_reader.c
	
main.o: main.c ../pf.h hashmap_reader.c
	cc -c main.c
