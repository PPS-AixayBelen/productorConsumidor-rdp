#ifndef _RDP_
#define _RDP_
#include "dataStructures.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRODUCTOR 2
#define CONSUMIDOR 3

#define PACKET_NUMBER 50
#define TRANSITIONS 4
#define PLACES 7
#define BUFFERS 2
#define DEBUG 0
#define ERROR -5
typedef struct rdp rdp_o;

/**
 * @brief Se declaran los metodos que tendra la estructura red de petri
 * 
 */
struct rdp_metodos
{

    int (*isPos)(rdp_o *, int *index);
    int (*ifEnd)(rdp_o *);
    void (*cleanRDP)(rdp_o *);
};

/**
 * @brief Estructura que representa la clase red de petri, contiene una estructura 
 * de tipo rdp_metodos que contiene una serie de punteros a las funciones que 
 * corresponden a la red de petri.
 * 
 */
struct rdp
{

    o_vector M;
    o_vector Sensitized;
    o_matriz Ineg;
    o_matriz I;
    int dataNumber;
    int packetCounter;
    void *invPlaza;

    const struct rdp_metodos *metodos;
};

// Funciones

int new_rdp(rdp_o *p_rdp);

#endif