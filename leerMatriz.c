#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
