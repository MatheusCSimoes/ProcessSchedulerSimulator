#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

//A structure to represent an I/O device
struct IOdevice {
  struct Queue *processes; //fila de processos para realizar IO
  int executed; //quanto já foi executado do processo no topo da fila
  int operationTime; //tempo da operacao de IO
};

struct IOdevice* createIOdevice(unsigned operTime, int maxProcesses) {
  struct IOdevice* device = (struct IOdevice*)malloc(sizeof(struct IOdevice));

  device->executed = 0;
  device->operationTime = operTime;
  device->processes = createQueue(maxProcesses);

  return device;
}

int enqueueIO(struct IOdevice* device, unsigned pid) {
  return (enqueue(device->processes, pid));
}

int remainingIO(struct IOdevice* device) {
  return (device->operationTime - device->executed);
}

//retorna o pid do processo caso tenha terminado a exec e -1 caso contrario
int execIO(struct IOdevice* device, int execTime) { //execTime = 1 para o simulador
  if(isEmpty(device->processes)) return -1;

  int remainingTime = remainingIO(device);

  if(execTime >= remainingTime) {
      device->executed = 0; //terminou a operacao e esta pronto para comecar uma nova
      return (dequeue(device->processes));
  }

  device->executed = device->executed + execTime;
  return -1;
}
