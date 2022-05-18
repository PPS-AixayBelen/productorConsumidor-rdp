#ifndef _POLITICA_
#define _POLITICA_

#include "rdp.h"

typedef struct politica politica_o;

struct politica_metodos
{

    int (*signalPolitic)(politica_o *, int *boolQuesWait);
};

struct politica
{

    rdp_o *rdp;
    int *listBuff;
    int *addBuffer;
    int *markVector;

    const struct politica_metodos *metodos;
};

// Funciones

void new_politica(politica_o *p_politica, rdp_o *rdp);

#endif