#include "monitor.h"

#include <stdio.h>
#include <stdlib.h>

void logInvarianteTransicion(monitor_o *monitor, int index)
{
    char *transicion[4] = {"T0", "T1", "T2", "T3"};
    if (monitor->logInvTransicion == NULL)
    {
        monitor->logInvTransicion = (char *)malloc(sizeof(char) * 3);
        strcpy(monitor->logInvTransicion, transicion[index]);
    }
    else
    {
        monitor->logInvTransicion = (char *)realloc(monitor->logInvTransicion, sizeof(char) * (strlen(monitor->logInvTransicion) + 3));
        strcat(monitor->logInvTransicion, transicion[index]);
    }
}

int verifyMInvariants(monitor_o *monitor)
{

    int *mark = &monitor->rdp->M.vector[0];

    if (((mark[0] + mark[4]) == 3) && ((mark[1] + mark[3]) == 2) && ((mark[3] + mark[4] + mark[5]) == 1) && ((mark[2] + mark[3] + mark[4] + mark[6]) == 5))
    {
        return 1;
    }
    else
        return 0; // rompiose
}

void cleanMonitor(monitor_o *monitor)
{
    free(monitor->politica);
    free(monitor->logInvTransicion);
}

void finalSignalPolitic(monitor_o *monitor) // Despierta a todos los hilos para terminar la ejecucion
{
    for (int i = 0; i < TRANSITIONS; i++)
    {
        pthread_cond_broadcast(&(monitor->espera[i]));
        monitor->boolQuesWait[i] = 0;
    }
}

void signalPoliticMonitor(monitor_o *monitor, int index) // define que hilo tiene que despertar y lo despierta
{
    if (monitor->rdp->metodos->ifEnd(monitor->rdp))
    { // Si la politica devuelve -1 es porque no pudo despertar a nadie, me fijo si tengo que terminar
        if (DEBUG)
            printf("No se pudo despertar a ningun hilo\n");
        monitor->end = 1;
        // for (int i = 0; i < TRANSITIONS; i++){
        //     pthread_cond_broadcast(&(monitor->espera[i])); //por si algun hilo quedo dormido
        // }

        finalSignalPolitic(monitor);
    }
    else
    {
        int t = monitor->politica->metodos->signalPolitic(monitor->politica, monitor->boolQuesWait, index); // Devuelve el indice de la transicion donde esta el hilo a despertar
        if (t != -1)
        {
            pthread_cond_signal(&(monitor->espera[t]));
            return;
        }
    }

    return;
}

int shoot(monitor_o *monitor, int index) // Dispara una transicion (index) devuelve 0 si pudo hacerla
{

    pthread_mutex_lock(&(monitor->mutex));

    int shoot[TRANSITIONS];
    for (int i = 0; i < TRANSITIONS; i++)
    {
        shoot[i] = 0;
    }
    shoot[index] = 1; // el vector de diisparo tiene un 1 solamente en la transicion que se va a disparar
    int shootResult = -1;

    while (1)
    {
        shootResult = monitor->rdp->metodos->isPos(monitor->rdp, shoot); // si el disparo es posible, lo realiza

        if (shootResult < 0) // si devolvio -1, el hilo deberia irse a dormir
        {
            if(shootResult==ERROR)
            {
                printf("ERROR DE ALOCACION. \n");
                exit(1);
            }
            if (monitor->end) // si ya se llego al final de la ejecucion, no se puede disparar nada
            {
                pthread_mutex_unlock(&(monitor->mutex));
                return -1;
            }

            if (DEBUG)
                printf("me fui a dormir disparando %d, con shootResult = %d \n", index, shootResult);

            monitor->boolQuesWait[index] += 1; // se setea un 1 en la transicion en la que se durmio el hilo
            pthread_cond_wait(&(monitor->espera[index]), &(monitor->mutex));
        }
        else if (shootResult == 0)
        {
            if (TEST_INVARIANTS)
            {
                logInvarianteTransicion(monitor, index);
            }

            if (monitor->boolQuesWait[index] > 0)
            {
                monitor->boolQuesWait[index] -= 1; // porque en este caso solo puede haber un hilo dormido por transicion
            }

            signalPoliticMonitor(monitor, index); // despierto al proximo hilo
            break;
        }
        else
        {
            pthread_mutex_unlock(&(monitor->mutex));
            return shootResult;
        }
    }

    if (TEST_INVARIANTS)
    {
        if (!verifyMInvariants(monitor))
        {
            printf("error de Invariantes\n");
            exit(1); // rompiose
        }
    }

    pthread_mutex_unlock(&(monitor->mutex));
    return 0;
}

struct monitor_metodos monitorMetodos = {

    .signalPoliticMonitor = signalPoliticMonitor,
    .finalSignalPolitic = finalSignalPolitic,
    .shoot = shoot,
    .cleanMonitor = cleanMonitor};

extern void new_monitor(monitor_o *p_monitor, pthread_mutex_t mutex, pthread_cond_t *espera, int *boolQuesWait, rdp_o *rdp)
{
    p_monitor->rdp = rdp;
    p_monitor->mutex = mutex;
    p_monitor->espera = espera;
    p_monitor->logInvTransicion = NULL;
    p_monitor->boolQuesWait = boolQuesWait;
    p_monitor->end = 0;
    p_monitor->metodos = &monitorMetodos;
    p_monitor->politica = (politica_o *)malloc(sizeof(politica_o));
    new_politica(p_monitor->politica, rdp);
}
