#ifndef _dataStructures_
#define _dataStructures_

#include <stdio.h>
#include <stdlib.h>

#define ALLOC_ERROR -1
#define ALLOC_OK 0

typedef struct vector o_vector;
typedef struct matriz o_matriz;

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

struct matriz_methods{
    int (*alloc_matriz)(o_matriz *);
	int (*free_matriz)(o_matriz *);
};

struct matriz{
    int **matriz;
    int filas;
    int columnas;
    struct matriz_methods * metodos;
};

extern int new_vector(o_vector * p_v, int v_size);
extern int new_matriz(o_matriz * p_m , int columnas, int filas);

#endif