#include <stdio.h>

#include "queue.h"
#include "process.h"

#define Nprocess 5
#define TimeSlice 3

struct Queue *high;
struct Queue *low;

struct Process *processes[Nprocess];

void init() {
  high = createQueue(Nprocess);
  low = createQueue(Nprocess);

  for(int i = 0; i < Nprocess; i++) {
    processes[i] = createProcess(5);
  }

  //inicialmente todos os processos vão começar no tempo 0
  for(int i = 0; i < Nprocess; i++) {
    enqueue(low, i); //alterar para pegar o id do processo
    printf("Processo %d foi inserido na fila de baixa prioridade\n", i);
  }
}

void checkNewProcess()
{
    //se novos processos iniciarem, coloca-los na fila
}

void runScheduler()
{
  int processToExec = Nprocess;
  int currentTime = 0;
  while(processToExec > 0) {
    checkNewProcess();
    int currentProcessId = -1;

    if(!isEmpty(high)) {
      currentProcessId = dequeue(high);
    }
    else {
      currentProcessId = dequeue(low);
    }

    if(currentProcessId < 0) {
      break;
    }

    int remainingTime = exec(processes[currentProcessId], TimeSlice);
    printf("Processo %d executou por %d unidades de tempo\n", currentProcessId, TimeSlice - remainingTime);
    currentTime = currentTime + TimeSlice - remainingTime;

    if(remaining(processes[currentProcessId]) == 0) {
      processToExec--;
      printf("O processo %d finalizou sua execucao\n", currentProcessId);
    }
    else {
      enqueue(low, currentProcessId);
      printf("Processo %d foi inserido na fila de baixa prioridade\n", currentProcessId);
    }
  }
}

int main()
{
  init();

  runScheduler();

  return 0;
}
