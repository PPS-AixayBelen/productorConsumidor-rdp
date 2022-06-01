#include "dataStructures.h"
#include <stdlib.h>

int alloc_vector(o_vector *p_vector);
void free_vector(o_vector *p_vector);
void print(o_vector p_vector);
int copy(o_vector dst, o_vector src);

int alloc_matriz(o_matriz *p_matriz);
void free_matriz(o_matriz *p_matriz);
int cargar_matriz_file(o_matriz *p_matriz, char *nombreArchivo);

struct vector_methods v_methods = {
    .alloc_vector = alloc_vector,
    .free_vector = free_vector,
    .print = print,
    .copy = copy};

struct matriz_methods m_methods = {
    .alloc_matriz = alloc_matriz,
    .free_matriz = free_matriz,
    .cargar_matriz_file = cargar_matriz_file};

/**
 * @brief Aloca memoria para un vector cuyo tamaño esta especificado en la variable size de p_vector.
 * 
 * @param p_vector Puntero a una estructura de tipo o_vector
 * @return int -1 si ocurrio un error en la alocacion, 0 si la alocacion fue correcta.
 */
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

/**
 * @brief Imprime por pantalla el vector, en el formato {1 2 3 4}
 * 
 * @param p_vector Puntero a la estructura con el vector a imprimir
 */
void print(o_vector p_vector)
{
    printf("{ ");
    for (int i = 0; i < p_vector.size; i++)
    {
        printf("%d ", p_vector.vector[i]);
    }
    printf("}\n");
}

/**
 * @brief Libera la variable vector de p_vector alocada con alloc_vector.
 * 
 * @param p_vector Estructura que contiene el vector a liberar.
 */
void free_vector(o_vector *p_vector)
{
    free(p_vector->vector);
}

/**
 * @brief Aloca memoria para una matriz cuyo tamaño esta especificado en p_matriz
 * 
 * @param p_matriz Puntero donde se alocara la matriz, contiene el tamaño.
 * @return int -1 si hubo un fallo en la alocacion, 0 si la alocacion fue correcta.
 */
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

/**
 * @brief Copia los valores de la variable vector de la estructura src en la variable vector de dst
 * 
 * @param dst Estructura destino
 * @param src Estructura desde donde se copia el vector
 * @return int -1 si el campo size de los vectores no es igual, 0 si se logro copiar exitosamente.
 */
int copy(o_vector dst, o_vector src)
{
    if (src.size != dst.size)
    {
        return -1;
    }

    for (int i = 0; i < dst.size; i++)
    {
        dst.vector[i] = src.vector[i];
    }

    return 0;
}

/**
 * @brief Carga desde un archivo de nombre nombreArchivo una matriz en la varible matriz de p_matriz
 * 
 * @param p_matriz Puntero a la estructura donde se debe cargar la matriz
 * @param nombreArchivo Cadena de caracteres con el nombre del archivo donde se encuentra la matriz
 * @return int -1 si ocurre un error al abrir el archivo, 0 si la carga se realiza correctamente.
 */
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

/**
 * @brief Libera la memoria alocada para la variable matriz de p_matriz
 * 
 * @param p_matriz Estructura que contiene el puntero a la memoria que debe ser liberada
 */
void free_matriz(o_matriz *p_matriz)
{
    for (int j = 0; j < p_matriz->filas; j++)
    {
        free(p_matriz->matriz[j]);
    }
    free(p_matriz->matriz);
}

/**
 * @brief Inicializa los valores de la estructura p_v, alocando la memoria necesaria.
 * 
 * @param p_v Puntero a la estructura o_vector
 * @param v_size Tamaño del vector 
 * @return int -1 si ocurre un error al alocar la memoria, 0 si se aloca correctamente
 */
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

/**
 * @brief Inicializa los valores en la estructura p_m, alocando la memoria necesaria para la matriz
 * 
 * @param p_m Estructura de tipo o_matriz.
 * @param filas 
 * @param columnas 
 * @return int -1 si ocurre un error al alocar la memoria, 0 si se aloca correctamente
 */
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
