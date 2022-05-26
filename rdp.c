#include "rdp.h"
#include "leerMatriz.h"
#include "time.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int isPos(rdp_o *rdp, int *index);
void updateTimeStamps(rdp_o *rdp, int *oldSens);
void getSensitized(rdp_o *rdp);
int ifEnd(rdp_o *rdp);
void cleanRDP(rdp_o *rdp);

void printArray(int size, int array[]);
void logInvariantePlaza(int *vectorMarcado, int size);

struct rdp_metodos rdpMetodos = {

    .isPos = isPos,
    .ifEnd = ifEnd,
    .cleanRDP = cleanRDP
    };

extern int new_rdp(rdp_o *p_rdp)
{
    p_rdp->dataNumber = 5; //cant max de paquetes a generar
    p_rdp->packetCounter = 0; //contador de paquetes generados hasta el momento por T0

    char M[] = "3 2 5 0 0 1 0"; //marcado inicial
    if(new_vector(&p_rdp->M,PLACES) == -1)
    {
        return -1;
    }

    if(new_vector(&p_rdp->Sensitized,TRANSITIONS) == -1)
    {
        return -1;
    }
    cargar_vector(PLACES, p_rdp->M.vector, M); 
    leer_matriz(PLACES, TRANSITIONS, p_rdp->Ineg[0], "Ineg");
    leer_matriz(PLACES, TRANSITIONS, p_rdp->Ipos[0], "Ipos");
    leer_matriz(PLACES, TRANSITIONS, p_rdp->I[0], "Imatriz"); // Matriz de incidencia (N x M)

    //TODO: Pasar a funcion que inicialice?
    for(int i = 0; i < TRANSITIONS; i++)
        p_rdp->Sensitized.vector[i] = 0;

    p_rdp->metodos = &rdpMetodos;
}

void cleanRDP(rdp_o *rdp)
{
    rdp->M.v_methods->free_vector(&rdp->M);
    rdp->Sensitized.v_methods->free_vector(&rdp->Sensitized);
}

int isPos(rdp_o *rdp, int *index)
{

    int temp;

    char *M_name[PLACES] = {"Consumidor", "Productor", "Huecos", "Produciendo", "Consumiendo", "Mutex", "Productos"};

    // Calculo E (vector de sensibilizado)
    for (int m = 0; m < TRANSITIONS; m++)
    {
        rdp->Sensitized.vector[m] = 1;

        for (int n = 0; n < PLACES; n++)
        {
            if (rdp->M.vector[n] - rdp->Ineg[n][m] < 0)
            {
                rdp->Sensitized.vector[m] = 0;
                break;
            }
        }
    }

    // Limitacion de generacion de datos (T0)
    if (rdp->packetCounter == rdp->dataNumber) // Desensibiliza T0 si ya termino de generar paquetes
        rdp->Sensitized.vector[0] = 0;

    int aux[TRANSITIONS];

    for (int i = 0; i < TRANSITIONS; i++)
    {
        aux[i] = rdp->Sensitized.vector[i]; // Vector de sensibilizadas y no inhibidas
    }

    for (int m = 0; m < TRANSITIONS; m++)
    {
        if (aux[m] * index[m] > 0)
            aux[m] = 1; // sigma and Ex
        else
            aux[m] = 0; // Si no pongo el else, quedan los unos de la operacion anterior
    }

    int zeroCounter = 0; // Esto es para ver que lo que quiero y puedo disparar sea diferente de 0
    for (int m = 0; m < TRANSITIONS; m++)
    {
        if (aux[m] != 0)
            zeroCounter++;
    }
  
    if (zeroCounter == 0){ 
        if (DEBUG)
            printf("vector de disparo vacio o insensibilizado\n");
        return -1;
    }

    int aux2[] = {0, 0, 0, 0, 0, 0, 0};

    for (int n = 0; n < PLACES; n++)
    {
        for (int m = 0; m < TRANSITIONS; m++)
        {
            temp = rdp->I[n][m] * aux[m];
            aux2[n] = aux2[n] + temp;
        }
    }

    int mPrima[PLACES] = {0, 0, 0, 0, 0, 0, 0};

    if (DEBUG)
        printf("Nuevo marcado: \n");
    for (int n = 0; n < PLACES; n++) // Si algun numero del nuevo vector de marcado es negativo, no puedo dispararla
    {                                    
        mPrima[n] = rdp->M.vector[n] + aux2[n]; // Sumo para obtener el nuevo vector de marcado
        if (DEBUG)
            printf("%d %s \n", mPrima[n], M_name[n]);

        if (mPrima[n] < 0)
        {
            if(DEBUG)
                printf("la transicion no se puede disparar, marcado resultante negativo\n");
            return -1;
        }
    }
    
    for (int i = 0; i < PLACES; i++)
    {
        rdp->M.vector[i] = mPrima[i];
    }


    if (index[0] == 1)
    {
        rdp->packetCounter = rdp->packetCounter+1;
        if (DEBUG)
            printf("\nNuevo paquete ahora tengo: %d\n",rdp->packetCounter);
    }

    // updateTimeStamps(rdp, oldSens); // Le mando el vector de sensiblizado del marcado anterior
    for (int m = 0; m < TRANSITIONS; m++)
    {
        rdp->Sensitized.vector[m] = 1;

        for (int n = 0; n < PLACES; n++)
        {
            if (rdp->M.vector[n] - rdp->Ineg[n][m] < 0)
            {
                rdp->Sensitized.vector[m] = 0;
                break;
            }
        }
    }
    if (DEBUG)
    {
        printf("Nuevo sensiblizado : ");
        printArray(TRANSITIONS, rdp->Sensitized.vector);
    }

    return 0;
}

void printArray(int size, int array[])
{
    printf("{ ");
    for (int i = 0; i < size; i++)
    {   
        printf("%d ", array[i]);
    }
    printf("}\n");
}



int ifEnd(rdp_o *rdp) //determina si ya volvi al marcado inicial y se generaron todos los paquetes requeridos
{
    int Minitial[PLACES] = {3,2,5,0,0,1,0};

    for (int n = 0; n < PLACES; n++)
    {  
        if (rdp->M.vector[n] != Minitial[n])
        {
            return 0;
        }
    }

    if (rdp->packetCounter == rdp->dataNumber)
    {
        return 1;
    }
    return 0;
}
