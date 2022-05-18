#include "politica.h"
#include "rdp.h"
#include <stdio.h>

int signalPolitic(politica_o *politica, int *boolQuesWait)
{

    //TODO: Revisar
    int *aux = politica->rdp->Sensitized;

    for (int i = 0; i < TRANSITIONS; i++){ //desperta al primero que encuentre dormido
        if(aux[i] == 1 && (boolQuesWait[i]>0) ){
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

    p_politica->markVector = p_politica->rdp->M;
    p_politica->metodos = &politicaMetodos;
}
