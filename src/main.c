#include <stdio.h>

#include "queue.h"
#include "process.h"
#include "iodevice.h"

#define Nprocess 5
#define SliceTime 3
#define IOdisco 5
#define IOfita 10
#define IOimpressora 15

struct Queue *high;
struct Queue *low;

struct IOdevice *disco;
struct IOdevice *fitaMagnetica;
struct IOdevice *impressora;

struct Process *processes[Nprocess];

void init() {
  high = createQueue(Nprocess);
  low = createQueue(Nprocess);

  disco = createIOdevice(IOdisco, Nprocess);
  fitaMagnetica = createIOdevice(IOfita, Nprocess);
  impressora = createIOdevice(IOimpressora, Nprocess);

  for(int i = 0; i < Nprocess; i++) {
    processes[i] = createProcess(i, 5);
  }

  //inicialmente todos os processos vão começar no tempo 0
  for(int i = 0; i < Nprocess; i++) {
    enqueue(low, i); //alterar para pegar o id do processo
    printf("t = 0: Processo %d foi inserido na fila de baixa prioridade\n", i);
  }
}

void checkNewProcess() {
    //se novos processos iniciarem, coloca-los na fila
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
    checkNewProcess(); //verifica se algum novo processo foi iniciado

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
      printf("t = %d: CPU ociosa.", currentTime);
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
