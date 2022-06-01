#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Lee una matriz del archivo con nombre nombreArchivo y carga los valores en matriz.
 * 
 * @param filas Filas de la matriz
 * @param columnas Columnas de la matriz
 * @param matriz Puntero a la matriz donde deben guardarse los valores leidos.
 * @param nombreArchivo Nombre del archivo desde donde debe leerse la matriz.
 * @return int 0 si ocurrio un error al abrir el archivo y 1 si la carga de
 *  la matriz se realizo correctamente.
 */
int leer_matriz(int filas, int columnas, int *matriz, char *nombreArchivo)
{

    FILE *archivo = fopen(nombreArchivo, "r");
    char linea[30];
    char *token;
    int temp;
    if (archivo == NULL)
    {
        printf("Error en apertura de archivo");
        return 0;
    }

    int k = 0;

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            fscanf(archivo, "%d", &temp);
            matriz[k] = temp;
            k++;
        }
    }

    fclose(archivo);
    return 1;
}

/**
 * @brief Carga el vector cuyos valores se encuentran en el vector v separados
 *  por un espacio en el vector.
 * 
 * @param size Tamaño del vector a cargar.
 * @param vector Vector donde deben guardarse los valores leidos
 * @param v Cadena de caracteres que contiene los valores del array separados por un espacio.
 */
void cargar_vector(int size, int *vector, char *v)
{

    char *token;

    token = strtok(v, " ");
    vector[0] = atoi(token);

    for (int i = 1; i < size; i++)
    {
        token = strtok(NULL, " ");
        vector[i] = atoi(token);
    }
}
