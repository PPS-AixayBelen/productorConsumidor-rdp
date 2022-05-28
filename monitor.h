#ifndef _MONITOR_
#define _MONITOR_

// Defining _REENTRANT causes the compiler to use thread safe (i.e. re-entrant)
// versions of several functions in the C library.
#define _REENTRANT
#include <pthread.h>
#include "rdp.h"
#include "politica.h"

typedef struct monitor monitor_o;

struct monitor_metodos
{

    void (*signalPoliticMonitor)(monitor_o *, int index);
    void (*finalSignalPolitic)(monitor_o *);
    int (*shoot)(monitor_o *, int index);
    void (*cleanMonitor)(monitor_o*);
};

struct monitor
{
    pthread_mutex_t mutex; 
    pthread_cond_t *espera; 
    int *boolQuesWait; //0 = no esta esperando , 1 = esta esperando
    politica_o *politica;
    rdp_o *rdp;
    int end;
    char *logInvTransicion;
    const struct monitor_metodos *metodos;
};

void new_monitor(monitor_o *p_monitor, pthread_mutex_t mutex, pthread_cond_t *espera, int *boolQuesWait, rdp_o *rdp);

#endif