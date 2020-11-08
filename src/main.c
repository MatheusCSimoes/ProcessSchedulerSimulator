#include <stdio.h>
#include <stdlib.h> //apenas para utilizar rand() para gerar tempos dos processos

#include "queue.h"
#include "process.h"
#include "iodevice.h"

#define Nprocess 5 //numero de processos para simulacao
#define SliceTime 3 //tamanho da fatia de tempo para execucao no algoritmo RR
#define IOdisco 5 //tempo para I/O em disco
#define IOfita 10 //tempo para I/O em fita magnetica
#define IOimpressora 15 //tempo para I/O em impressora

Queue *high;
Queue *low;

IOdevice *disco;
IOdevice *fitaMagnetica;
IOdevice *impressora;

Process *processes[Nprocess];

int currentTime = 0;
int remainingSliceTime = SliceTime;
int currentProcessId = -1;

void init() {
  high = createQueue(Nprocess);
  low = createQueue(Nprocess);

  disco = createIOdevice(IOdisco, Nprocess);
  fitaMagnetica = createIOdevice(IOfita, Nprocess);
  impressora = createIOdevice(IOimpressora, Nprocess);

  printf("Criando processos:\n");
  for(int i = 0; i < Nprocess; i++) {
    int execTime = (rand() % 20) + 5;
    int startTime = (rand() % 50);
    processes[i] = createProcess(i, execTime, startTime); //id, tempo de exec, tempo de chegada
    printf("Processo %d: tempo de inicio = %d, tempo de execucao = %d.\n", i, startTime, execTime);
  }
  printf("\n");
}

void checkNewProcess(int currentTime) { //se novos processos iniciarem, coloca-los na fila
  for(int i = 0; i < Nprocess; i++) {
    if(processes[i]->startTime == currentTime) {
      enqueue(high, i); //coloca "id" do processo na fila
      printf("t = %d: Processo %d foi iniciado e inserido na fila de alta prioridade\n", currentTime, i);
    }
  }
}

void getNewProcessId() {
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
}

void checkIOdevices() {
  int requestIO = (rand() % 100) > 90; //90% de chance de um processo pedir I/O em cada instante de tempo
  if(requestIO && currentProcessId >= 0) {
    int IOtype = (rand() % 3); //33% de chance do I/O ser de cada tipo

    switch (IOtype) {
      case 0 /*impressora*/:
        enqueueIO(impressora, currentProcessId);
        printf("t = %d: Processo %d inserido na fila de I/O para impressora.\n", currentTime, currentProcessId);
        break;
      case 1 /*fita*/:
        enqueueIO(fitaMagnetica, currentProcessId);
        printf("t = %d: Processo %d inserido na fila de I/O para fita magnetica.\n", currentTime, currentProcessId);
        break;
      case 2 /*disco*/:
        enqueueIO(disco, currentProcessId);
        printf("t = %d: Processo %d inserido na fila de I/O para disco.\n", currentTime, currentProcessId);
        break;
    }

    currentProcessId = -1;
    getNewProcessId(); //pega novo processo para executar enquanto o atual é passado para IO
  }
}

void runScheduler() {
  int processToExec = Nprocess;

  currentTime = 0;
  remainingSliceTime = SliceTime;
  currentProcessId = -1;

  while(processToExec > 0) {
    checkNewProcess(currentTime); //verifica se algum novo processo foi iniciado

    getNewProcessId();

    checkIOdevices(); //verifica andamento das operacoes de IO

    currentTime = currentTime + 1;

    //execucao dos I/Os
    int pidDisco = execIO(disco, 1);
    if(pidDisco >= 0) {
      printf("t = %d: Processo %d terminou I/O em disco e é inserido na fila de baixa prioridade.\n", currentTime, pidDisco);
      enqueue(low, pidDisco);
    }

    int pidFita = execIO(fitaMagnetica, 1);
    if(pidFita >= 0) {
      printf("t = %d: Processo %d terminou I/O em fita magnetica e é inserido na fila de alta prioridade.\n", currentTime, pidFita);
      enqueue(high, pidFita);
    }

    int pidImpressora = execIO(impressora, 1);
    if(pidImpressora >= 0) {
      printf("t = %d: Processo %d terminou I/O em impressora e é inserido na fila de alta prioridade.\n", currentTime, pidImpressora);
      enqueue(high, pidImpressora);
    }

    if(currentProcessId < 0) { //verifica se tem algum processo para ser executado pela CPU
      printf("t = %d: CPU ociosa.\n", currentTime);
      continue;
    }

    exec(processes[currentProcessId], 1); //executa o processo

    if(remaining(processes[currentProcessId]) == 0) { //verifica se o processo terminou execucao completa
      printf("t = %d: O processo %d finalizou sua execucao\n", currentTime, currentProcessId);
      processToExec--;
      currentProcessId = -1;
    }
    else {
      if(remainingSliceTime - 1 == 0) { //verifica se a fatia de tempo para o processo acabou
        enqueue(low, currentProcessId);
        printf("t = %d: Processo %d foi inserido na fila de baixa prioridade\n", currentTime, currentProcessId);
        currentProcessId = -1;
      }
      else {
        remainingSliceTime = remainingSliceTime - 1;
      }
    }
  }
}

int main() {
  init();

  runScheduler();

  return 0;
}
