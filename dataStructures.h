#ifndef _dataStructures_
#define _dataStructures_

#include <stdio.h>
#include <stdlib.h>
typedef struct vector o_vector;

struct vector_methods{
    int (*alloc_vector)(o_vector *);
	int (*free_vector)(o_vector *);
    void (*print)(o_vector );
    // int (*create_zero_vector)(o_vector *,int);
	// int (*create_one_vector)(o_vector *,int);
};

struct vector{
    int * vector;
    int size;
    const struct vector_methods * v_methods;
};

extern int new_vector(o_vector * p_v, int v_size);

#endif