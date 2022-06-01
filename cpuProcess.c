#include "cpuProcess.h"
#include <stdio.h>
#include <unistd.h>

/**
 * @brief Dispara las transiciones especificadas en la variable processList de cpuProcess.
 * 
 * @param cpuProcess Estructura que contiene la lista de transiciones a disparar y el numero de estas
 * @return void* NULL cuando el hilo debe terminar.
 */
void *run(cpuProcess_o *cpuProcess)
{

    if (DEBUG)
        printf("VOY A DISPARAR: %d y soy el hilo con id %d\n", cpuProcess->processList[0], cpuProcess->id);
    // sleep(1);
    int shootResult = 0;
    while (1)
    {
        for (int i = 0; i < (cpuProcess->processNum); i++)
        {
            if (DEBUG)
                printf("Nro de transicion a disparar: %d y soy el hilo con id %d\n", cpuProcess->processList[i], cpuProcess->id);
            shootResult = cpuProcess->monitor->metodos->shoot(cpuProcess->monitor, cpuProcess->processList[i]);

            if (DEBUG)
                printf("shootResult - %d disparo- %d\n", shootResult, cpuProcess->processList[i]);

            if (shootResult == -1)
            {
                if (DEBUG)
                    printf("Finalizado hilo con id: %d\n", cpuProcess->id);
                return NULL;
            }
        }
    }
}

struct cpuProcess_metodos cpuMetodos = {
    .run = run};

/**
 * @brief Inicializa los valores de la estructura p_cpuProcess
 * 
 * @param p_cpuProcess Puntero a la estructura a inicializar
 * @param processList Vector con los indices de las transiciones a disparar
 * @param processNum Cantidad de transiciones que debe disparar
 * @param monitor Puntero al monitor que acciona la red de petri y dispara transiciones
 * @param id Id del hilo
 */
extern void new_cpuProcess(cpuProcess_o *p_cpuProcess, int *processList, int processNum, monitor_o *monitor, int id)
{
    p_cpuProcess->monitor = monitor;
    p_cpuProcess->processNum = processNum;
    p_cpuProcess->metodos = &cpuMetodos;
    p_cpuProcess->processList = processList;
    p_cpuProcess->id = id;
}
