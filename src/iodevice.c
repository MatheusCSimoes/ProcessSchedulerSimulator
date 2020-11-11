#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "iodevice.h"

IOdevice* createIOdevice(unsigned operTime, int maxProcesses) {
  IOdevice* device = (IOdevice*)malloc(sizeof(IOdevice));

  device->executed = 0;
  device->operationTime = operTime;
  device->processes = createQueue(maxProcesses);

  return device;
}

int enqueueIO(IOdevice* device, unsigned pid) {
  return (enqueue(device->processes, pid));
}

int remainingIO(IOdevice* device) {
  return (device->operationTime - device->executed);
}

int currentProcess(IOdevice* device) {
  return (front(device->processes));
}

//retorna o pid do processo caso tenha terminado a exec e -1 caso contrario
int execIO(IOdevice* device, int execTime) { //execTime = 1 para o simulador
  if(isEmpty(device->processes)) return -1;

  int remainingTime = remainingIO(device);

  if(execTime > remainingTime) {
      device->executed = 0; //terminou a operacao e esta pronto para comecar uma nova
      int temp = (dequeue(device->processes));
      if(!isEmpty(device->processes))
      	device->executed = device->executed + execTime;
		  
	  return temp;
  }

  device->executed = device->executed + execTime;
  return -2;
}
