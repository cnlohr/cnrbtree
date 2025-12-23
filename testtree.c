#define FULLTEST
#define ITERATIONS 300

struct cnrbtree_generic_node_t;
int PrintTreeRootIt( struct cnrbtree_generic_node_t * t );

#define CNRBTREE_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "cnrbtree.h"


#define tstrcopy(x,y,z) { x = strdup(y); z.a = 0; z.b = 0; }
#define tstrdelete(x,y) free( x );

typedef struct some_payload_t
{
	int a;
	int b;
} payload;
typedef char * str_;

CNRBTREETEMPLATE( str_ , payload, RBstrcmp, tstrcopy, RBstrdel )
CNRBTREETEMPLATE( str_ , int, RBstrcmp, RBstrcpy, tstrdelete )

int anotherfn()
{
	cnrbtree_str_payload  * tree = cnrbtree_str_payload_create();

	RBA( tree, "hello" ).a = 5;
	RBA( tree, "world" ).b = 6;
	RBA( tree, "how" ).a = 9;
	RBA( tree, "are" ).b = 4;
	RBA( tree, "you" ).a = 3;
	RBA( tree, "a" );
	RBA( tree, "b" );
	RBA( tree, "c" );
	RBA( tree, "d" );
	RBA( tree, "e" );
	RBA( tree, "f" );
	RBA( tree, "g" );
	RBA( tree, "h" );

	printf( "%d\n", RBA( tree, "hello" ).a );
	printf( "%d\n", RBA( tree, "how" ).a );
	printf( "%d\n", RBA( tree, "are" ).b );
	printf( "%d\n", RBA( tree, "you" ).a );
	printf( "%d\n", RBA( tree, "world" ).b );

	cnrbtree_str_payload_remove( tree, "are" );

	RBFOREACH( str_payload, tree, i )
	{
		printf( ".key = %5s   .a = %d .b = %d\n", i->key, i->data.a, i->data.b );
	}

	RBDESTROY( tree );
	return 0;
}

int gints = 0;

#define intcomp(x, y) ( x - y )
#define intcopy(x, y, z) (x = y, gints++, z = 0 ) 
#define intdelete( x, y )     gints--;

#define intcomp(x, y) ( x - y )

// an unalign-unsafe-version of RBstrcmp
//#define strtest( x, y ) ((*((int64_t*)x)) == (*((int64_t*)y))?0: (((*((int64_t*)x)) - (*((int64_t*)y)))) < 0 ? - 1 : 1 )

typedef char * str;
CNRBTREETEMPLATE( int, int, intcomp, intcopy, intdelete )
CNRBTREETEMPLATE( str, str, RBstrcmp, RBstrcpy, RBstrdel )

int PrintTree( cnrbtree_intint_node * t, int depth, cnrbtree_intint_node * parent );

int PrintTreeRootIt( cnrbtree_generic_node * t )
{
#ifndef FULLTEST
	return 0;
#endif
	while( t->parent->left != t->parent ) t = t->parent;
	PrintTree( (cnrbtree_intint_node *)t, -10000, (cnrbtree_intint_node*)t->parent );
	return 0;
}


int PrintTree( cnrbtree_intint_node * t, int depth, cnrbtree_intint_node * parent )
{
#ifndef FULLTEST
	return 0;
#endif
	int stordepth = depth;
	if( depth < 0 ) depth += 10000;

	if( t->left == t )
	{
		printf( "%*s-\n", depth*4, "" );
		return 1;
	}
	printf( "%*s%d %d (%p) PARENT: %p\n", depth*4, "", t->key, t->color, (void*)t, (void*)t->parent );
	int d1 = PrintTree( t->left, stordepth+1, t );
	int d2 = PrintTree( t->right, stordepth+1, t );
	if( d1 != d2 && stordepth >= 0 )
	{
		fprintf( stderr, "Black Fault (%d != %d)\n", d1, d2 );
		exit(1);
	}
	if( stordepth >= 0 && t->color == CNRBTREE_COLOR_RED && t->parent->left != t->parent && t->parent->color == CNRBTREE_COLOR_RED )
	{
		fprintf( stderr, "Red Fault\n" );
		exit(1);
	}
	if( stordepth >= 0 && t->parent != parent )
	{
		fprintf( stderr, "Parent fault [%p %p]\n", (void*)t->parent, (void*)parent );
		exit( 1);
	}
	return (depth < 0)?-1:(d1 + (t->color == CNRBTREE_COLOR_BLACK));
	/*
		Every node is either red or black.
		Every leaf (NULL) is black.
		If a node is red, then both its children are black.
		Every simple path from a node to a descendant leaf contains the same number of black nodes. 
	*/
}

int main()
{
	{
		cnrbtree_strstr * tree = cnrbtree_strstr_create();

		srand(0);
		int i, j;
		for( j = 0; j < 3000; j++ )
		{
			char stta[ITERATIONS][9]  __attribute__((aligned(8)));
			for( i = 0; i < ITERATIONS; i++ )
			{
				int k;
				for( k = 0; k < 8; k++ ) 
					stta[i][k] = (rand()%26) + 'a';
				stta[i][k] = 0;

				if( cnrbtree_strstr_get( tree, stta[i] ) )
				{
					printf( "Duplicate.  Try again.\n" );
					exit( 5 );
				}

				cnrbtree_strstr_access( tree, stta[i] )->data = strdup( stta[i] );
				// Could also be
				// RBA( tree, stta[i] ) = strdup( stta[i] );
			}

			if( tree->size != ITERATIONS)
			{
				printf( "Size violation. %d\n", tree->size );
				exit( 5 );
			}

			for( i = 0; i < ITERATIONS; i++ )
			{
				cnrbtree_strstr_node * n = cnrbtree_strstr_get( tree, stta[i] );
				if( !n )
				{
					printf( "Access fault on %s (%d)\n", stta[i], i );
					exit( 5 );
				}
				free( n->data );
				cnrbtree_strstr_remove( tree, stta[i] );
			}
asm volatile( "" : : : "memory" );
			if( !RBISNIL( tree->node ) )
			{
				printf( "Excess fault %p\n", (void*)tree->node );
				exit( 6 );
			}
			if( tree->size != 0 )
			{
				printf( "Size violation. %d\n", tree->size );
				exit( 5 );
			}

		}

		cnrbtree_strstr_destroy( tree );
		printf( "OK\n" );

	}
	{
		cnrbtree_intint * tree;
		tree = cnrbtree_intint_create();
		srand(0);
		int addlist[ITERATIONS];
		int i;
		int j;
		for( j = 0; j < 4; j++ )
		{
			for( i = 0; i < ITERATIONS; i++ )
			{
				PrintTree( tree->node, 0, tree->node->parent );
		retry:
				addlist[i] = rand()%(ITERATIONS*10);
			//	printf( "Adding: %d\n", (addlist[i] = rand()) );
				if( cnrbtree_intint_get( tree, addlist[i] ) )
				{
					printf( "Duplicate.  Try again.\n" );
					goto retry;
				}

				cnrbtree_intint_access( tree, addlist[i] )->data = 80;
				// Could also be
				//RBA( tree, addlist[i] ) = 80;

				printf( "SIZE: %d\n", tree->size );
			}

			if( tree->size != ITERATIONS)
			{
				printf( "Size violation. %d\n", tree->size );
				exit( 5 );
			}
			printf( "FINAL TREE BEFORE REMOVAL\n" );
			PrintTree( tree->node, 0, tree->node->parent );

			for( i = 0; i < ITERATIONS; i++ )
			{
				int k = addlist[i];
				printf( "Deleting %d (%d)\n", k, i );
				if( !cnrbtree_intint_get( tree, addlist[i] ) )
				{
					printf( "Missing fault on %d (%d)\n", addlist[i], i );
					exit( 5 );
				}
				cnrbtree_intint_remove( tree, k );
			//	printf( "Deleted %d\n", k );
				PrintTree( tree->node, 0, tree->node->parent );
			}
			if( tree->node != tree->node->left )
			{
				printf( "Excess fault\n" );
				exit( 6 );
			}
			if( tree->size != 0 )
			{
				printf( "Size violation. %d\n", tree->size );
				exit( 5 );
			}

		}



		for( i = 0; i < ITERATIONS; i++ )
		{
			PrintTree( tree->node, 0, tree->node->parent );
	retry2:
			printf( "Adding: %d\n", (addlist[i] = rand()) );
			if( cnrbtree_intint_get( tree, addlist[i] ) )
			{
				printf( "Duplicate.  Try again.\n" );
				goto retry2;
			}


			cnrbtree_intint_access( tree, addlist[i] )->data = 80;
			printf( "SIZE: %d\n", tree->size );
		}

		cnrbtree_intint_destroy( tree );

		if( gints != 0 )
		{
			printf( "gints fault\n" );
			exit( -6 );
		}
	}

int anotherfn();
	anotherfn();
	fprintf( stderr, "OK\n" );
}

