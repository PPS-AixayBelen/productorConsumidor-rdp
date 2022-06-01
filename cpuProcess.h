#ifndef _CPUPROCESS_
#define _CPUPROCESS_

#include "monitor.h"

typedef struct cpuProcess cpuProcess_o;

struct cpuProcess_metodos
{

    void *(*run)(cpuProcess_o *);
};

/**
 * @brief Estructura que contiene la funcion que ejecutaran los hilos
 * e informacion sobre las transiciones que deben disparar los mismos.
 * 
 */
struct cpuProcess
{

    int processNum; // Para probar
    monitor_o *monitor;
    int *processList; // puntero a lista de transiciones
    int id;
    const struct cpuProcess_metodos *metodos;
};

// Funciones

void new_cpuProcess(cpuProcess_o *p_cpuProcess, int *processList, int processNum, monitor_o *monitor, int id);

#endif