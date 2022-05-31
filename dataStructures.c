#include "dataStructures.h"
#include <stdlib.h>

int alloc_vector(o_vector *p_vector);
int free_vector(o_vector *p_vector);
void print(o_vector p_vector);
int copy(o_vector dst,o_vector src);

int alloc_matriz(o_matriz *p_matriz);
void free_matriz(o_matriz *p_matriz);
int cargar_matriz_file(o_matriz *p_matriz, char *nombreArchivo);

struct vector_methods v_methods = {
    .alloc_vector = alloc_vector,
    .free_vector = free_vector,
    .print = print,
    .copy = copy
    };

struct matriz_methods m_methods = {
    .alloc_matriz = alloc_matriz,
    .free_matriz = free_matriz,
    .cargar_matriz_file = cargar_matriz_file
    };

int alloc_vector(o_vector *p_vector)
{

    int *p_v = (int *)malloc(sizeof(int) * p_vector->size);

    if (p_vector == NULL)
    {
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

int free_vector(o_vector *p_vector)
{
    free(p_vector->vector);
}

int alloc_matriz(o_matriz *p_matriz)
{
    int **matriz = (int **)malloc(p_matriz->filas * sizeof(int *));
    if (matriz == NULL)
    {
        return ALLOC_ERROR;
    }

    for (int i = 0; i < p_matriz->filas; i++)
    {
        matriz[i] = (int *)malloc(p_matriz->columnas * sizeof(int));
        if (matriz[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(matriz[j]);
            }
            free(matriz);
            return ALLOC_ERROR;
        }
    }
    p_matriz->matriz = matriz;
    return ALLOC_OK;
}


int copy(o_vector dst,o_vector src)
{
    if(src.size != dst.size)
    {
        return -1;
    }

    for (int i = 0; i < dst.size ; i++)
    {     
        dst.vector[i] = src.vector[i];
    }

    return 0;
}

int cargar_matriz_file(o_matriz *p_matriz, char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    char linea[30];
    char *token;
    int temp;
    if (archivo == NULL)
    {
        printf("Error en apertura de archivo");
        return READ_FILE_ERROR;
    }

    for (int i = 0; i < p_matriz->filas; i++)
    {
        for (int j = 0; j < p_matriz->columnas; j++)
        {
            fscanf(archivo, "%d", &temp);
            p_matriz->matriz[i][j] = temp;
            
        }
    }

    fclose(archivo);
    return READ_FILE_OK;
}

void free_matriz(o_matriz *p_matriz)
{
    for (int j = 0; j < p_matriz->filas; j++)
    {
        free(p_matriz->matriz[j]);
    }
    free(p_matriz->matriz);
}

extern int new_vector(o_vector *p_v, int v_size)
{
    p_v->size = v_size;
    p_v->v_methods = &v_methods;
    int p_v_alloc = p_v->v_methods->alloc_vector(p_v);
    if (p_v_alloc == ALLOC_ERROR)
    {
        return ALLOC_ERROR;
    }

    return ALLOC_OK;
}

extern int new_matriz(o_matriz *p_m, int filas, int columnas)
{
    p_m->columnas = columnas;
    p_m->filas = filas;
    p_m->metodos = &m_methods;

    int p_m_alloc = p_m->metodos->alloc_matriz(p_m);
    if (p_m_alloc == ALLOC_ERROR)
    {
        return ALLOC_ERROR;
    }

    return ALLOC_OK;
}
