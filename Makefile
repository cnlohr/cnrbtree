all : testtree

testtree : testtree.c
	gcc -o $@ $^ -O3 -g -Wall -Wno-unused-value -Wno-unused-function

test : testtree
	./testtree

clean :
	rm -rf *.o *~ testtree

