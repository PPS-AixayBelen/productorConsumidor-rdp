#include "politica.h"
#include "rdp.h"
#include <stdio.h>

/**
 * @brief Retorna el indice de la cola de condicion de donde se debe despertar un hilo. Si la transicion
 *  disparada corresponde a un productor, se debe despertar un hilo consumidor y viceversa.
 *
 * @param politica Puntero a la estructura politica
 * @param boolQuesWait Vector que contiene el numero de hilos bloqueados en cada cola de condicion
 * @param index Indice de la ultima transicion disparada
 * @return int Retorna el indice de la cola de condicion de donde se debe despertar un hilo, o -1
 *  si no se puede despertar a ninguno.
 */
int signalPolitic(politica_o *politica, int *boolQuesWait, int index)
{
    int i = 0;          //
    if (index % 2 == 0) // si la transicion es par, es del productor
        i = 1;          // y tiene que despertar a un consumidor

    int *aux = politica->rdp->Sensitized.vector;
    for (i; i < TRANSITIONS; i += 2)
    { // desperta al primero que encuentre dormido
        if (aux[i] == 1 && (boolQuesWait[i] > 0))
        {
            return i;
        }
    }

    return -1;
}

/**
 * @brief Estructura que contiene los metodos de la estructura politica.
 * 
 */
struct politica_metodos politicaMetodos = {

    .signalPolitic = signalPolitic

};

/**
 * @brief Se encarga de inicializar las variables de la estructura politica
 * 
 * @param p_politica Puntero a la estructura de la politica.
 * @param rdp Puntero a la estructura de la red de petri.
 */
extern void new_politica(politica_o *p_politica, rdp_o *rdp)
{
    p_politica->rdp = rdp;
    p_politica->metodos = &politicaMetodos;
}
