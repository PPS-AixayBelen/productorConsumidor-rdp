#include "politica.h"
#include "rdp.h"
#include <stdio.h>

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

struct politica_metodos politicaMetodos = {

    .signalPolitic = signalPolitic

};

extern void new_politica(politica_o *p_politica, rdp_o *rdp)
{
    p_politica->rdp = rdp;
    p_politica->markVector = p_politica->rdp->M.vector;
    p_politica->metodos = &politicaMetodos;
}
