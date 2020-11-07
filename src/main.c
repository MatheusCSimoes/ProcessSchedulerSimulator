#include <stdio.h>

#include "queue.h"
#include "process.h"
#include "iodevice.h"

#define Nprocess 5
#define SliceTime 3
#define IOdisco 5
#define IOfita 10
#define IOimpressora 15

Queue *high;
Queue *low;

IOdevice *disco;
IOdevice *fitaMagnetica;
IOdevice *impressora;

Process *processes[Nprocess];

void init() {
  high = createQueue(Nprocess);
  low = createQueue(Nprocess);

  disco = createIOdevice(IOdisco, Nprocess);
  fitaMagnetica = createIOdevice(IOfita, Nprocess);
  impressora = createIOdevice(IOimpressora, Nprocess);

  for(int i = 0; i < Nprocess; i++) {
    processes[i] = createProcess(i, 5, 10*i); //id, tempo de exec, tempo de chegada
  }
}

void checkNewProcess(int currentTime) { //se novos processos iniciarem, coloca-los na fila
  for(int i = 0; i < Nprocess; i++) {
    if(processes[i]->startTime == currentTime) {
      enqueue(high, i); //coloca "id" do processo na fila
      printf("t = %d: Processo %d foi iniciado e inserido na fila de alta prioridade\n", currentTime, i);
    }
  }
}

void checkIOdevices() {
    //verifica andamento das operacoes de IO
}

void runScheduler() {
  int processToExec = Nprocess;
  int currentTime = 0;
  int remainingSliceTime = SliceTime;
  int currentProcessId = -1;

  while(processToExec > 0) {
    checkNewProcess(currentTime); //verifica se algum novo processo foi iniciado

    if(currentProcessId == -1) {
      if(!isEmpty(high)) {
        currentProcessId = dequeue(high);
      }
      else {
        currentProcessId = dequeue(low);
      }

      remainingSliceTime = SliceTime;

      if(currentProcessId >= 0) {
        printf("t = %d: Processo %d retirado da fila e entra em execucao\n", currentTime, currentProcessId);
      }
    }

    currentTime = currentTime + 1;

    if(currentProcessId < 0) {
      printf("t = %d: CPU ociosa.\n", currentTime);
      continue;
    }

    exec(processes[currentProcessId], 1); //executa o processo

    if(remaining(processes[currentProcessId]) == 0) {
      printf("t = %d: O processo %d finalizou sua execucao\n", currentTime, currentProcessId);
      processToExec--;
      currentProcessId = -1;
    }
    else {
      if(remainingSliceTime - 1 == 0) {
        enqueue(low, currentProcessId);
        printf("t = %d: Processo %d foi inserido na fila de baixa prioridade\n", currentTime, currentProcessId);
        currentProcessId = -1;
      }
      else {
        remainingSliceTime = remainingSliceTime - 1;
      }
    }

    checkIOdevices(); //verifica andamento das operacoes de IO
  }
}

int main() {
  init();

  runScheduler();

  return 0;
}
