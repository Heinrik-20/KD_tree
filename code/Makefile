map2: main2.o data.o items.o utils.o tree.o map.o read.o
	gcc -Wall -o map2 main2.o data.o items.o utils.o tree.o map.o read.o -lm -g
	
map1: main1.o data.o items.o utils.o tree.o map.o read.o
	gcc -Wall -o map1 main1.o data.o items.o utils.o tree.o map.o read.o -lm -g
	
main2.o: main2.c
	gcc -Wall -c -o main2.o main2.c -g

main1.o: main1.c
	gcc -Wall -c -o main1.o main1.c -g
	
data.o: data.c data.h
	gcc -Wall -c -o data.o data.c -g
	
items.o: items.c items.h
	gcc -Wall -c -o items.o items.c -g

utils.o: utils.c utils.h
	gcc -Wall -c -o utils.o utils.c -g	
	
tree.o: tree.c tree.h
	gcc -Wall -c -o tree.o tree.c -g
	
map.o: map.c map.h
	gcc -Wall -c -o map.o map.c -g

read.o: read.c read.h
	gcc -Wall -c -o read.o read.c -g
	
clean1:
	rm -f *.o
	rm map1

clean2:
	rm -f *.o
	rm map2