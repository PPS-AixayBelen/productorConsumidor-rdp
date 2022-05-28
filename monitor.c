#include "monitor.h"

#include <stdio.h>
#include <stdlib.h>

void logInvariantePlaza(int *vectorMarcado, int size)
{
    FILE *invPlaza = fopen("./test/InvariantesPlaza", "a+");

    for (int i = 0; i < size; i++)
    {
        fprintf(invPlaza, "%d ", vectorMarcado[i]);
    }
    fputs("\n", invPlaza);
    fclose(invPlaza);
}

void logInvarianteTransicion(monitor_o *monitor, int index)
{
    char *transicion[] = {"T0", "T1", "T2", "T3"};
    if (monitor->logInvTransicion == NULL)
    {
        monitor->logInvTransicion = (char *)malloc(sizeof(char) * strlen(transicion[index]+1));
        strcpy(monitor->logInvTransicion, transicion[index]);
    }
    else
    {
        monitor->logInvTransicion = (char *)realloc(monitor->logInvTransicion, sizeof(char) * (strlen(monitor->logInvTransicion) +1+ strlen(transicion[index])));
        strcat(monitor->logInvTransicion, transicion[index]);
    }
}

void cleanMonitor(monitor_o *monitor){
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
            logInvariantePlaza(&monitor->rdp->M.vector[0], PLACES);
            //logInvarianteTransicion(monitor, index);

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
