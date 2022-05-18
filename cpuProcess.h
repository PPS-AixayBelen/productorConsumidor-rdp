#ifndef _CPUPROCESS_
#define _CPUPROCESS_

#include "monitor.h"

typedef struct cpuProcess cpuProcess_o;

struct cpuProcess_metodos
{

    void *(*run)(cpuProcess_o *);
};

struct cpuProcess
{

    int processNum; // Para probar
    monitor_o *monitor;
    int *processList; // puntero a lista de transiciones
    const struct cpuProcess_metodos *metodos;
};

// Funciones

void new_cpuProcess(cpuProcess_o *p_cpuProcess, int *processList, int processNum, monitor_o *monitor);

#endif