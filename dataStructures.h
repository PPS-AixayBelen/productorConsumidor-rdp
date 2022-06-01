#ifndef _dataStructures_
#define _dataStructures_

#include <stdio.h>
#include <stdlib.h>

#define ALLOC_ERROR -1
#define ALLOC_OK 0
#define READ_FILE_OK 0
#define READ_FILE_ERROR -1

typedef struct vector o_vector;
typedef struct matriz o_matriz;

struct vector_methods
{
    int (*alloc_vector)(o_vector *);
    int (*free_vector)(o_vector *);
    void (*print)(o_vector);
    int (*copy)(o_vector dst, o_vector src);
};

struct vector
{
    int *vector;
    int size;
    const struct vector_methods *v_methods;
};

struct matriz_methods
{
    int (*alloc_matriz)(o_matriz *);
    void (*free_matriz)(o_matriz *);
    int (*cargar_matriz_file)(o_matriz *, char *);
};

struct matriz
{
    int **matriz;
    int filas;
    int columnas;
    struct matriz_methods *metodos;
};

extern int new_vector(o_vector *p_v, int v_size);
extern int new_matriz(o_matriz *p_m, int columnas, int filas);

#endif