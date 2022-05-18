#ifndef _RDP_
#define _RDP_
#include "time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRANSITIONS 15
#define PLACES 16
#define BUFFERS 2
#define DEBUG 1
#define PRODUCTOR 1
typedef struct rdp rdp_o;

struct rdp_metodos
{

    int (*isPos)(rdp_o *, int *index);
    void (*updateTimeStamps)(rdp_o *, int *oldSens);
    void (*getSensitized)(rdp_o *);
    int (*ifEnd)(rdp_o *);
};

struct rdp{
   
    int M[PLACES]; 
    int B[TRANSITIONS]; 
    int Ipos[PLACES][TRANSITIONS]; 
    int Ineg[PLACES][TRANSITIONS]; 
    int I[PLACES][TRANSITIONS];  
    int H[TRANSITIONS][PLACES]; 
    int E[TRANSITIONS]; 
    int Sensitized[TRANSITIONS]; 

    time_t sensitizedTime[TRANSITIONS];
    int minTimeArrival; 
    int minTimeSrv1; 
    int minTimeSrv2; 
    int isBuffer[BUFFERS];
    int isGenTransition[PRODUCTOR];
    int isAddBuffer[BUFFERS];

    int dataNumber;
    int packetCounter; // Es boolean
    void *invPlaza;

    const struct rdp_metodos *metodos;
};

// Funciones

void new_rdp(rdp_o *p_rdp);

#endif