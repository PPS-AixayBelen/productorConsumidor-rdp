#include "politica.h"
#include "rdp.h"
#include <stdio.h>

int signalPolitic(politica_o *politica, int *boolQuesWait)
{

    int *aux = politica->rdp->Sensitized;
    if (boolQuesWait[politica->addBuffer[0]] && boolQuesWait[politica->addBuffer[1]] && aux[politica->addBuffer[0]] == 1) // si t1 y t8 estan dormidas y esta sensibilizada t1
    { 
        politica->markVector = politica->rdp->M;
        if (politica->markVector[politica->listBuff[0]] < politica->markVector[politica->listBuff[1]]) //si el buffer 1 tiene menos cosas que el buffer 2, usa el buffer 1 
        {
            return politica->addBuffer[0];
        }
        else if (politica->markVector[politica->listBuff[0]] >= politica->markVector[politica->listBuff[1]]) // sino, usa el buffer 1
        {
            return politica->addBuffer[1];
        }
    }
    for (int i = 0; i < TRANSITIONS; i++){ //desperta al primero que encuentre dormido y que no sean t1 ni t8
        if(aux[i] == 1 && boolQuesWait[i] && i !=5 && i != 13){
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
    p_politica->listBuff = p_politica->rdp->isBuffer;     // la lista de los nros de plaza que representan a cpu_buffer y cpu_buffer2
    p_politica->addBuffer = p_politica->rdp->isAddBuffer; // la lista de los nros que representan las transiciones que cargan los buffers (t1 y t8)
    p_politica->markVector = p_politica->rdp->M;
    p_politica->metodos = &politicaMetodos;
}
