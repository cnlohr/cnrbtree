all : testtree

testtree : testtree.c
	gcc -o $@ $^ -Os -g -Wall -Wno-unused-value -Wno-unused-function -Wextra -pedantic -fsanitize=address,undefined
# -Wstrict-aliasing=3
# -fno-strict-aliasing
# -fsanitize=type 

simple : simple.c
	gcc -o $@ $^ -Os -g

test : testtree simple
	./testtree
	./simple

clean :
	rm -rf *.o *~ testtree

