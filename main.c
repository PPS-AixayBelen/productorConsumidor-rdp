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


int main()
{
    
    pthread_attr_t atrib;
    pthread_t c[15];

    pthread_attr_init(&atrib);
    pthread_attr_setscope(&atrib, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setschedpolicy(&atrib, SCHED_RR);
    
    monitor_o mymonitor;
    rdp_o rdp;
    monitor_o *monitor = &mymonitor;
    cpuProcess_o tShooter[TRANSITIONS];

    int *shootArray[15];

    int shootSequence1[1]={0};      //dispara T0 
    int shootSequence2[1]={5};      //dispara T1
    int shootSequence3[1]={13};     //dispara T8
    int shootSequence4[1]={11};     //dispara T6 (Power UP CPU1)
    int shootSequence5[1]={12};     //dispara T7 (CPU ON 1)
    int shootSequence6[1]={7};      //T13 POWER UP 2
    int shootSequence7[1]={6};      //T12
    int shootSequence8[1]={9};      //T2
    int shootSequence9[1]={3};      //Service rate 1
    int shootSequence10[1]={1};     //Power down 1
    int shootSequence11[1]={2};     //Power down 2
    int shootSequence12[1]={14};    //T9
    int shootSequence13[1]={4};     //Service rate 2
    int shootSequence14[1]={8};     //T14
    int shootSequence15[1]={10};    //T5

    shootArray[0] = shootSequence1;
    shootArray[1] = shootSequence2;
    shootArray[2] = shootSequence3;
    shootArray[3] = shootSequence4;
    shootArray[4] = shootSequence5;
    shootArray[5] = shootSequence6;
    shootArray[6] = shootSequence7;
    shootArray[7] = shootSequence8;
    shootArray[8] = shootSequence9;
    shootArray[9] = shootSequence10;
    shootArray[10] = shootSequence11;
    shootArray[11] = shootSequence12;
    shootArray[12] = shootSequence13;
    shootArray[13] = shootSequence14;
    shootArray[14] = shootSequence15;
     
    pthread_mutex_t mutex; 
    pthread_cond_t espera[TRANSITIONS];
    int boolQuesWait[TRANSITIONS] = {0};

    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 15; i++)
    {
        pthread_cond_init(&espera[i], NULL);
    }
    new_rdp(&rdp);
    new_monitor(&mymonitor, mutex, espera, boolQuesWait, &rdp);

    for (int i = 0; i < TRANSITIONS; i++)
    {
        new_cpuProcess(&tShooter[i], shootArray[i], 1, monitor);
    }

    for (int i = 0; i < TRANSITIONS; i++)
        pthread_create(&c[i], &atrib,(&tShooter[i])->metodos->run , (cpuProcess_o *) &tShooter[i]);

    for (int i = 0; i < TRANSITIONS; i++){
        pthread_join(c[i], NULL);
    }
    if (DEBUG)
        printf("\nFinalizo la ejecucion\n");
        
    FILE * invTransicionFile = fopen("./test/InvarianteTransicion","w+");
    fputs(monitor->logInvTransicion,invTransicionFile);
    fclose(invTransicionFile);

    free(monitor->politica);
    free(monitor->logInvTransicion);
}
