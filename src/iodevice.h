#ifndef IODEVICE_H
#define IODEVICE_H

//A structure to represent an I/O device
typedef struct _IOdevice {
  Queue *processes; //fila de processos para realizar IO
  int executed; //quanto já foi executado do processo no topo da fila
  int operationTime; //tempo da operacao de IO
} IOdevice;

IOdevice* createIOdevice(unsigned execTime, int maxProcesses);
int enqueueIO(IOdevice* device, unsigned pid);
int remainingIO(IOdevice* device);
int execIO(IOdevice* device, int execTime);
int currentProcess(IOdevice* device);

#endif	
