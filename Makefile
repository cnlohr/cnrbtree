all : testtree

testtree : testtree.c
	gcc -o $@ $^ -O2 -g -Wall -Wno-unused-value -Wno-unused-function -Wextra -pedantic -fsanitize=address,undefined

test : testtree
	./testtree

clean :
	rm -rf *.o *~ testtree

