// Defining _REENTRANT causes the compiler to use thread safe (i.e. re-entrant)
// versions of several functions in the C library.
#define _REENTRANT

#include "cpuProcess.h"
#include "monitor.h"
#include "politica.h"
#include "rdp.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PRODUCTOR 2
#define CONSUMIDOR 3


int main()
{
    
    pthread_attr_t atrib;
    pthread_t c[5];

    pthread_attr_init(&atrib);
    pthread_attr_setscope(&atrib, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setschedpolicy(&atrib, SCHED_RR);
    
    monitor_o mymonitor;
    rdp_o rdp;
    monitor_o *monitor = &mymonitor;
    cpuProcess_o tShooter[5];

    int *shootArray[2];

    int shootSequence1[2]={0,2};      //Productor
    int shootSequence2[2]={1,3};      //Consumidor

    shootArray[0] = shootSequence1;
    shootArray[1] = shootSequence2;

    pthread_mutex_t mutex; 
    pthread_cond_t espera[TRANSITIONS];
    int boolQuesWait[TRANSITIONS] = {0};

    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < TRANSITIONS; i++)
    {
        pthread_cond_init(&espera[i], NULL);
    }
    new_rdp(&rdp);
    new_monitor(&mymonitor, mutex, espera, boolQuesWait, &rdp);

    for (int i = 0; i < PRODUCTOR;i++)
    {
        new_cpuProcess(&tShooter[i], shootArray[0], 2, monitor);
    }

        for (int i = 2; i <5;i++)
    {
        new_cpuProcess(&tShooter[i], shootArray[1], 2, monitor);
    }

    for (int i = 0; i < (PRODUCTOR + CONSUMIDOR); i++)
        pthread_create(&c[i], &atrib,(void*)(&tShooter[i])->metodos->run , (cpuProcess_o *) &tShooter[i]);

    for (int i = 0; i < (PRODUCTOR + CONSUMIDOR); i++){
        pthread_join(c[i], NULL);
    }

   
    if (DEBUG)
        printf("\nFinalizo la ejecucion\n");
    
    monitor->metodos->cleanMonitor(monitor);
    rdp.metodos->cleanRDP(&rdp);
    
}
