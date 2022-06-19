#ifndef _MONITOR_
#define _MONITOR_

// Defining _REENTRANT causes the compiler to use thread safe (i.e. re-entrant)
// versions of several functions in the C library.
#define _REENTRANT
#include <pthread.h>
#include "rdp.h"
#include "politica.h"
#define TEST_INVARIANTS 0

typedef struct monitor monitor_o;

/**
 * @brief Estructura que define punteros a las funciones para la estructura monitor.
 * 
 */
struct monitor_metodos
{

    void (*signalPoliticMonitor)(monitor_o *, int index);
    void (*finalSignalPolitic)(monitor_o *);
    int (*shoot)(monitor_o *, int index);
    void (*cleanMonitor)(monitor_o *);
    int (*verifyMInvariants)(monitor_o *monitor);
};

/**
 * @brief Estructura que contiene las siguientes variables:
 * mutex: Mutex para asegurar la exclusion mutua en el acceso al monitor.
 * espera: Vector que contiene una cola de condicion por transicion.
 * boolQuesWait: Vector que indica cuantos hilos hay actualmente durmiendo 
 * en cada cola de condicion de espera.
 * politica: Estructura que corresponde a una clase politica.
 * rdp: Estructura que corresponde a una clase rdp.
 * end: Indica si el programa debe finalizar cuando su valor es 1.
 * logInvTransicion: String que contiene las transiciones disparadas.
 * metodos: Puntero a una estructura con punteros a funciones.
 */
struct monitor
{
    pthread_mutex_t mutex;
    pthread_cond_t *espera;
    int *boolQuesWait; // 0 = no esta esperando , 1 = esta esperando
    politica_o *politica;
    rdp_o *rdp;
    int end;
    char *logInvTransicion;
    const struct monitor_metodos *metodos;
};

int new_monitor(monitor_o *p_monitor, pthread_mutex_t mutex, pthread_cond_t *espera, int *boolQuesWait, rdp_o *rdp);

#endif