#include "dataStructures.h"
#include <stdlib.h>

int alloc_vector(o_vector * p_vector);
int free_vector(o_vector p_vector);

struct vector_methods v_methods ={
    .alloc_vector = alloc_vector,
    //.free_vector = free_vector
    };

int alloc_vector(o_vector * p_vector){

    int * p_v = (int *) malloc(sizeof(int)*p_vector->size);

    if(p_vector == NULL){
        return -1;
    }

    p_vector->vector = p_v;
    return 0;
}

int free_vector(o_vector p_vector){
    free(p_vector.vector);
}

extern int new_vector(o_vector * p_v, int v_size){
    p_v->size=v_size;
    p_v->v_methods =&v_methods;
    int p_v_alloc = p_v->v_methods->alloc_vector(p_v);
    if(p_v_alloc == -1)
    {
        return -1;
    }

    return 0;
}