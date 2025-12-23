#include <stdio.h>

#define CNRBTREE_IMPLEMENTATION
#include "cnrbtree.h"

typedef struct object_t
{
	int myvalue;
} object;
typedef char * str;
CNRBTREETEMPLATE( str, object, RBstrcmp, RBstrcpy, RBstrdel );

int main(void)
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

