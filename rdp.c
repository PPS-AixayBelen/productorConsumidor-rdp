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
    if(new_vector(&p_rdp->M,PLACES) == ALLOC_ERROR)
    {
        return ALLOC_ERROR;
    }

    if(new_vector(&p_rdp->Sensitized,TRANSITIONS) == ALLOC_ERROR)
    {
        return ALLOC_ERROR;
    }
    cargar_vector(PLACES, p_rdp->M.vector, M); 

    if(new_matriz(&p_rdp->Ineg,PLACES,TRANSITIONS) == ALLOC_ERROR)
    {
        return ALLOC_ERROR;
    }
    p_rdp->Ineg.metodos->cargar_matriz_file(&p_rdp->Ineg,"Ineg");
    
    if(new_matriz(&p_rdp->I,PLACES,TRANSITIONS) == ALLOC_ERROR)
    {
        return ALLOC_ERROR;
    }
    p_rdp->Ineg.metodos->cargar_matriz_file(&p_rdp->I,"Imatriz");

    //TODO: Pasar a funcion que inicialice?
    for(int i = 0; i < TRANSITIONS; i++)
        p_rdp->Sensitized.vector[i] = 0;

    p_rdp->metodos = &rdpMetodos;
}

void cleanRDP(rdp_o *rdp)
{
    rdp->M.v_methods->free_vector(&rdp->M);
    rdp->Sensitized.v_methods->free_vector(&rdp->Sensitized);
    rdp->Ineg.metodos->free_matriz(&rdp->Ineg);
    rdp->I.metodos->free_matriz(&rdp->I);
}

int isPos(rdp_o *rdp, int *index)
{

    int temp;

    char *M_name[PLACES] = {"Consumidor", "Productor", "Huecos", "Produciendo", "Consumiendo", "Mutex", "Productos"};

    for (int m = 0; m < TRANSITIONS; m++)
    {
        rdp->Sensitized.vector[m] = 1;

        for (int n = 0; n < PLACES; n++)
        {
            if (rdp->M.vector[n] - rdp->Ineg.matriz[n][m] < 0) //TODO: Rompe aca
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
            temp = rdp->I.matriz[n][m] * aux[m];
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

    for (int m = 0; m < TRANSITIONS; m++)
    {
        rdp->Sensitized.vector[m] = 1;

        for (int n = 0; n < PLACES; n++)
        {
            if (rdp->M.vector[n] - rdp->Ineg.matriz[n][m] < 0)
            {
                rdp->Sensitized.vector[m] = 0;
                break;
            }
        }
    }
    if (DEBUG)
    {
        printf("Nuevo sensiblizado : ");
        rdp->Sensitized.v_methods->print(rdp->Sensitized);
    }

    return 0;
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
