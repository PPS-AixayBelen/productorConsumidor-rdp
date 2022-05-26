#ifndef _RDP_
#define _RDP_
#include "dataStructures.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRODUCTOR 2
#define CONSUMIDOR 3

#define TRANSITIONS 4
#define PLACES 7
#define BUFFERS 2
#define DEBUG 1
//#define PRODUCTOR 1
typedef struct rdp rdp_o;

struct rdp_metodos
{

    int (*isPos)(rdp_o *, int *index);
    int (*ifEnd)(rdp_o *);
    void (*cleanRDP)(rdp_o *);
};

struct rdp{
   
    o_vector M; 
    o_vector Sensitized; 
    int Ipos[PLACES][TRANSITIONS]; 
    int Ineg[PLACES][TRANSITIONS]; 
    int I[PLACES][TRANSITIONS];  
    int H[TRANSITIONS][PLACES]; 
    int E[TRANSITIONS]; 
   

    int dataNumber;
    int packetCounter; // Es boolean
    void *invPlaza;

    const struct rdp_metodos *metodos;
};

// Funciones

int new_rdp(rdp_o *p_rdp);

#endif