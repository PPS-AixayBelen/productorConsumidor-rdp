#include "dataStructures.h"
#include <stdlib.h>


int alloc_vector(o_vector * p_vector);
int free_vector(o_vector *p_vector);
void print(o_vector p_vector);

int alloc_matriz(o_matriz * p_matriz);
int free_matriz(o_matriz *p_matriz);

struct vector_methods v_methods ={
    .alloc_vector = alloc_vector,
    .free_vector = free_vector,
    .print = print
    };

struct matriz_methods m_methods ={
    .alloc_matriz = alloc_matriz,
    .free_matriz = free_matriz
    };


int alloc_vector(o_vector * p_vector){

    int * p_v = (int *) malloc(sizeof(int)*p_vector->size);

    if(p_vector == NULL){
        return ALLOC_ERROR;
    }

    p_vector->vector = p_v;
    return ALLOC_OK;
}

void print(o_vector p_vector)
{
    printf("{ ");
    for (int i = 0; i < p_vector.size; i++)
    {   
        printf("%d ", p_vector.vector[i]);
    }
    printf("}\n");
}


int free_vector(o_vector *p_vector){
    free(p_vector->vector);
}

int alloc_matriz(o_matriz * p_matriz)
{
    int ** matriz = (int **) malloc(p_matriz->filas*sizeof(int*));
    if(matriz==NULL)
    {
        return ALLOC_ERROR;
    }

    for(int i = 0;i<p_matriz->filas;i++)
    {
        matriz[i] = (int*) malloc(p_matriz->columnas*sizeof(int));
        if(matriz[i]==NULL)
        {
            //TODO: ROLLBACK
            return ALLOC_ERROR;
        }
    }
    p_matriz->matriz = matriz;
    return ALLOC_OK;
}


extern int new_vector(o_vector * p_v, int v_size){
    p_v->size=v_size;
    p_v->v_methods =&v_methods;
    int p_v_alloc = p_v->v_methods->alloc_vector(p_v);
    if(p_v_alloc == ALLOC_ERROR)
    {
        return ALLOC_ERROR;
    }

    return ALLOC_OK;
}


