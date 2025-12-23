all : testtree

testtree : testtree.c
	gcc -o $@ $^ -Os -g -Wall -Wno-unused-value -Wno-unused-function -Wextra -pedantic -fsanitize=address,undefined
# -Wstrict-aliasing=3
# -fno-strict-aliasing
# -fsanitize=type 

test : testtree
	./testtree

clean :
	rm -rf *.o *~ testtree

