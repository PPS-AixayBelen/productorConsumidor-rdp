#include "rdp.h"
//#include "leerMatriz.h"
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
void free_aux_data_structures();

/**
 * @brief Estructura que contiene los metodos que tenga la estructura red de petri.
 * 
 */
struct rdp_metodos rdpMetodos = {

    .isPos = isPos,
    .ifEnd = ifEnd,
    .cleanRDP = cleanRDP};

/**
 * @brief Inicializa las estructuras y variables que contiene la estructura que representa la red de petri,
 *  aloca los vectores y matrices necesarias.
 *
 * @param p_rdp Puntero a la estructura de la red de petri a inicializar
 * @return int -1 en caso de que ocurra un error al alocar las estructuras de datos, 0 si todo es correcto.
 */
extern int new_rdp(rdp_o *p_rdp)
{
    p_rdp->dataNumber = PACKET_NUMBER;    // cant max de paquetes a generar
    p_rdp->packetCounter = 0; // contador de paquetes generados hasta el momento por T0

    char M[] = "3 2 5 0 0 1 0"; // marcado inicial
    if (new_vector(&p_rdp->M, PLACES) == ALLOC_ERROR)
    {
        return ALLOC_ERROR;
    }

    if (new_vector(&p_rdp->Sensitized, TRANSITIONS) == ALLOC_ERROR)
    {
        return ALLOC_ERROR;
    }
    p_rdp->M.v_methods->cargar_vector(&p_rdp->M, M);
    if (new_matriz(&p_rdp->Ineg, PLACES, TRANSITIONS) == ALLOC_ERROR)
    {
        return ALLOC_ERROR;
    }
    p_rdp->Ineg.metodos->cargar_matriz_file(&p_rdp->Ineg, "Ineg");

    if (new_matriz(&p_rdp->I, PLACES, TRANSITIONS) == ALLOC_ERROR)
    {
        return ALLOC_ERROR;
    }
    p_rdp->Ineg.metodos->cargar_matriz_file(&p_rdp->I, "Imatriz");

    // TODO: Pasar a funcion que inicialice?
    for (int i = 0; i < TRANSITIONS; i++)
        p_rdp->Sensitized.vector[i] = 0;

    p_rdp->metodos = &rdpMetodos;
}

/**
 * @brief Se encarga de liberar las estructuras de datos alocadas por la funcion new_rdp.
 *
 * @param rdp Puntero a la estructura que contiene las estructuras a liberar.
 */
void cleanRDP(rdp_o *rdp)
{
    rdp->M.v_methods->free_vector(&rdp->M);
    rdp->Sensitized.v_methods->free_vector(&rdp->Sensitized);
    rdp->Ineg.metodos->free_matriz(&rdp->Ineg);
    rdp->I.metodos->free_matriz(&rdp->I);
}

/**
 * @brief Verifica si es posible disparar la transicion cuyo indice esta indicado por la variable index.
 *
 * @param rdp Puntero a la estructura de la red de petri
 * @param index Indice de la transicion a disparar
 * @return int -1 si no es posible disparar la transicion, 0 si es posible dispararla y -5 si ocurre un error
 * al intentar alocar los vectores.
 */
int isPos(rdp_o *rdp, int *index)
{

    int temp;

    char *M_name[PLACES] = {"Consumidor", "Productor", "Huecos", "Produciendo", "Consumiendo", "Mutex", "Productos"};

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

    // Limitacion de generacion de datos (T0)
    if (rdp->packetCounter == rdp->dataNumber) // Desensibiliza T0 si ya termino de generar paquetes
        rdp->Sensitized.vector[0] = 0;

    o_vector aux;
    if(new_vector(&aux, TRANSITIONS)==ALLOC_ERROR)
    {
        return ERROR;
    }

    aux.v_methods->copy(aux, rdp->Sensitized);

    for (int m = 0; m < TRANSITIONS; m++)
    {
        if (aux.vector[m] * index[m] > 0)
            aux.vector[m] = 1; // sigma and Ex
        else
            aux.vector[m] = 0; // Si no pongo el else, quedan los unos de la operacion anterior
    }

    int zeroCounter = 0; // Esto es para ver que lo que quiero y puedo disparar sea diferente de 0
    for (int m = 0; m < TRANSITIONS; m++)
    {
        if (aux.vector[m] != 0)
            zeroCounter++;
    }

    if (zeroCounter == 0)
    {
        if (DEBUG)
            printf("vector de disparo vacio o insensibilizado\n");
        aux.v_methods->free_vector(&aux);
        return -1;
    }

    o_vector aux2;
    if(new_vector(&aux2, PLACES)==ALLOC_ERROR)
    {
        return ERROR;
    }
    // TODO: Pasar a otra funcion?
    for (int i = 0; i < PLACES; i++)
    {
        aux2.vector[i] = 0;
    }

    for (int n = 0; n < PLACES; n++)
    {
        for (int m = 0; m < TRANSITIONS; m++)
        {
            temp = rdp->I.matriz[n][m] * aux.vector[m];
            aux2.vector[n] = aux2.vector[n] + temp;
        }
    }

    o_vector mPrima;
    if(new_vector(&mPrima, PLACES)==ALLOC_ERROR)
    {
        return ERROR;
    }

    if (DEBUG)
        printf("Nuevo marcado: \n");
    for (int n = 0; n < PLACES; n++) // Si algun numero del nuevo vector de marcado es negativo, no puedo dispararla
    {
        mPrima.vector[n] = rdp->M.vector[n] + aux2.vector[n]; // Sumo para obtener el nuevo vector de marcado
        if (DEBUG)
            printf("%d %s \n", mPrima.vector[n], M_name[n]);

        if (mPrima.vector[n] < 0)
        {
            if (DEBUG)
                printf("la transicion no se puede disparar, marcado resultante negativo\n");
            aux.v_methods->free_vector(&aux);
            aux2.v_methods->free_vector(&aux2);
            mPrima.v_methods->free_vector(&mPrima);
            return -1;
        }
    }

    for (int i = 0; i < PLACES; i++)
    {
        rdp->M.vector[i] = mPrima.vector[i];
    }

    if (index[0] == 1)
    {
        rdp->packetCounter = rdp->packetCounter + 1;
        if (DEBUG)
            printf("\nNuevo paquete ahora tengo: %d\n", rdp->packetCounter);
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

    aux.v_methods->free_vector(&aux);
    aux2.v_methods->free_vector(&aux2);
    mPrima.v_methods->free_vector(&mPrima);
    return 0;
}

/**
 * @brief Verifica si se generaron todos los paquetes generados y si se volvio al 
 * marcado inicial, indica que el programa debe terminar.
 * 
 * @param rdp Puntero a la estructura de la red de petri.
 * @return int 0 si el programa no debe terminar, 1 si el programa debe terminar.
 */
int ifEnd(rdp_o *rdp) // determina si ya volvi al marcado inicial y se generaron todos los paquetes requeridos
{
    int Minitial[PLACES] = {3, 2, 5, 0, 0, 1, 0};

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
