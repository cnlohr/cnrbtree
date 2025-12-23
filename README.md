# cnrbtree

A fully-templated red black tree in C, which can be used as a dictionary or a set.  Like like stl map, in C, as a single-file header, with convenience functionality for strings, ints, and indexing.

It has a particular focus on versatility and runtime speed, at the cost of code size.  For instance, it uses the while-style operations instead of recursive operations for speed, and all things like pointer compares are templated into the macro-generated code instead of using function pointers.  Even though the code is under 1k LOC, it incurs additional code size per new dictionary-type you create.

All as a single-file header, which you can use like:

```c
#include <...>
#include <...>
#define CNRBTREE_IMPLEMENTATION
#include "cnrbtree.h"
```

It has a number of conveniences like `RBHAS` and `RBA` to, in a type-ambivelant way ask a tree if it has a specific element, or, to be able to inline access a specific element.

```c
     if( RBHAS( mytree, mykey ) )
        RBA( mytree, mykey )++;
     else
        RBA( mytree, mykey ) = 1;
```

While I've been using this library all over my programming professionally and personally since about 2019, that doesn't mean there can't be bugs.  If you find any, please report them, along with a repro if at all possible.

## Example

```c
#include <stdio.h>

#define CNRBTREE_IMPLEMENTATION
#include "cnrbtree.h"

typedef struct object_t
{
	int myvalue;
} object;
typedef char * str;
CNRBTREETEMPLATE( str, object, RBstrcmp, RBstrcpy, RBstrdel );

int main()
{
	char * si, * sd;

	//Construct a generic string-nothing set.  (there is no "data")
	cnstrset * set = cnstrset_create();
	RBA( set, "hello" );
	RBA( set, "world" );
	cnstrset_foreach( set, si )
	{
		printf( "%s\n", si );
	}
	cnstrset_remove( set, "world" );
	cnstrset_foreach( set, si )
	{
		printf( "%s\n", si );
	}
	cnstrset_destroy( set );

	//Construct a generic string-string dictionary.
	cnstrstrmap * dict = cnstrstrmap_create();
	RBA( dict, "hello" ) = "hi";
	RBA( dict, "world" ) = "globe";
	cnstrstrmap_foreach( dict, si, sd )
	{
		printf( "%s %s\n", si, sd );
	}
	cnstrstrmap_remove( dict, "world" );
	cnstrstrmap_foreach( dict, si, sd )
	{
		printf( "%s %s\n", si, sd );
	}
	cnstrstrmap_destroy( dict );

	// Custom trees

	//Constructs our custom tree
	cnrbtree_strobject * tree = cnrbtree_strobject_create();

	//Accesses, like C++ map's [] operator.
	RBA( tree, "a" ).myvalue = 5;
	RBA( tree, "d" ).myvalue = 8;
	RBA( tree, "c" ).myvalue = 7;
	RBA( tree, "b" ).myvalue = 6;

	//Access, like [] but reading.
	printf( "%d == 7\n", RBA(tree, "c").myvalue );
	printf( "%d == 1\n", RBHAS( tree, "d" ) );
	printf( "%d == 0\n", RBHAS( tree, "z" ) );

	//Iterate through them all.
	RBFOREACH( strobject, tree, i )
	{
		printf( ".key = %s .myvalue = %d\n", i->key, i->data.myvalue );
	}

	//Typesafe delete.
	RBDESTROY( tree );

	return 0;
}
```

