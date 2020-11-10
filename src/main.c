#include <stdio.h>
#include <stdlib.h> //apenas para utilizar rand() para gerar tempos dos processos
#include <time.h> //para gerar o seed do rand()

#include "queue.h"
#include "process.h"
#include "iodevice.h"

#define Nprocess 5 //numero de processos para simulacao
#define SliceTime 3 //tamanho da fatia de tempo para execucao no algoritmo RR
#define IOdisco 5 //tempo para I/O em disco
#define IOfita 10 //tempo para I/O em fita magnetica
#define IOimpressora 15 //tempo para I/O em impressora

#define TAM_MAX 10000 //tempo para I/O em impressora

Queue *high;
Queue *low;

IOdevice *disco;
IOdevice *fitaMagnetica;
IOdevice *impressora;

char tabela[Nprocess+1][TAM_MAX][5];

Process *processes[Nprocess+1];

int currentTime = 0;
int remainingSliceTime = SliceTime;
int currentProcessId = -1;
int timeTabela = -1;

int io[Nprocess];

FILE *fp;

void init() {
  high = createQueue(Nprocess);
  low = createQueue(Nprocess);

  disco = createIOdevice(IOdisco, Nprocess);
  fitaMagnetica = createIOdevice(IOfita, Nprocess);
  impressora = createIOdevice(IOimpressora, Nprocess);

  printf("Criando processos:\n");
  for(int i = 1; i <= Nprocess; i++) {
    int execTime = (rand() % 20) + 5;
    int startTime = (rand() % 50);
    processes[i] = createProcess(i, execTime, startTime); //id, tempo de exec, tempo de chegada
    printf("Processo %d: Chegada no tempo = %d, tempo de servico = %d.\n", i, startTime, execTime);
  }
  printf("\n");
}

void checkNewProcess(int currentTime) { //se novos processos iniciarem, coloca-los na fila
  for(int i = 1; i <= Nprocess; i++) {
    if(processes[i]->startTime == currentTime) {
      enqueue(high, processes[i]->id); //coloca "id" do processo na fila
      printf("t = %d: Processo %d foi iniciado e inserido na fila de alta prioridade.\n", currentTime, processes[i]->id);
    }
  }
}

void getNewProcessId() {
  if(currentProcessId == -1) {
    if(!isEmpty(high)) {
      currentProcessId = dequeue(high);
      if(currentProcessId >= 0) {
        printf("t = %d: Processo %d retirado da fila de alta prioridade e entra em execucao.\n", currentTime, currentProcessId);
      }
    }
    else {
      currentProcessId = dequeue(low);
      if(currentProcessId >= 0) {
        printf("t = %d: Processo %d retirado da fila de baixa prioridade e entra em execucao.\n", currentTime, currentProcessId);
      }
    }
    if(currentProcessId > -1)
    	sprintf(tabela[currentProcessId][timeTabela], "%i", currentProcessId);

    remainingSliceTime = SliceTime;
  }
}

void checkIOdevices() {
  int requestIO = (rand() % 100) > 90; //90% de chance de um processo pedir I/O em cada instante de tempo
  if(requestIO && currentProcessId >= 0) {
    int IOtype = (rand() % 3); //33% de chance do I/O ser de cada tipo

    switch (IOtype) {
      case 0 /*impressora*/:
        enqueueIO(impressora, currentProcessId);
        io[0] = currentProcessId;
        printf("t = %d: Processo %d inserido na fila de I/O para impressora.\n", currentTime, currentProcessId);
        //sprintf(tabela[currentProcessId][timeTabela], "C");
        break;
      case 1 /*fita*/:
        enqueueIO(fitaMagnetica, currentProcessId);
        io[1] = currentProcessId;
        printf("t = %d: Processo %d inserido na fila de I/O para fita magnetica.\n", currentTime, currentProcessId);
        //sprintf(tabela[currentProcessId][timeTabela], "B");
        break;
      case 2 /*disco*/:
        enqueueIO(disco, currentProcessId);
        io[2] = currentProcessId;
        printf("t = %d: Processo %d inserido na fila de I/O para disco.\n", currentTime, currentProcessId);
        //sprintf(tabela[currentProcessId][timeTabela], "A");
        break;
    }

    currentProcessId = -1;
    //getNewProcessId(); //pega novo processo para executar enquanto o atual e passado para IO
  }
}

void runScheduler() {
  int processToExec = Nprocess;

  currentTime = 0;
  remainingSliceTime = SliceTime;
  currentProcessId = -1;


  while(processToExec > 0) {
  	timeTabela += 1;
  	sprintf(tabela[0][timeTabela], "%d;", currentTime);
  	for(int j = 1; j <= Nprocess; j++) {
	    sprintf(tabela[j][timeTabela], ";");
	}
    checkNewProcess(currentTime); //verifica se algum novo processo foi iniciado

    //checkIOdevices(); //verifica andamento das operacoes de IO

    //execucao dos I/Os
    int pidDisco = execIO(disco, 1);
    if(pidDisco == -2) {
    	sprintf(tabela[io[2]][timeTabela], "A;");
    }
    else if(pidDisco >= 0) {
      printf("t = %d: Processo %d terminou I/O em disco e foi inserido na fila de baixa prioridade.\n", currentTime, pidDisco);
      enqueue(low, pidDisco);
    }

    int pidFita = execIO(fitaMagnetica, 1);
    if(pidFita == -2) {
    	sprintf(tabela[io[1]][timeTabela], "B;");
    }
    else if(pidFita >= 0) {
      printf("t = %d: Processo %d terminou I/O em fita magnetica e foi inserido na fila de alta prioridade.\n", currentTime, pidFita);
      enqueue(high, pidFita);
    }

    int pidImpressora = execIO(impressora, 1);
    if(pidImpressora == -2) {
    	sprintf(tabela[io[0]][timeTabela], "C;");
    }
    else if(pidImpressora >= 0) {
      printf("t = %d: Processo %d terminou I/O em impressora e foi inserido na fila de alta prioridade.\n", currentTime, pidImpressora);
      enqueue(high, pidImpressora);
    }

    getNewProcessId();

	if(currentProcessId < 0) { //verifica se tem algum processo para ser executado pela CPU
      printf("t = %d: CPU ociosa.\n", currentTime);
      currentTime = currentTime + 1;
      continue;
    }
    else{
    	exec(processes[currentProcessId], 1); //executa o processo
    	sprintf(tabela[currentProcessId][timeTabela], "%d;", currentProcessId);
	}

    currentTime = currentTime + 1;

    checkIOdevices(); //verifica andamento das operacoes de IO

  	if(currentProcessId > -1) {
      if(remaining(processes[currentProcessId]) == 0) { //verifica se o processo terminou execucao completa
        printf("t = %d: O processo %d finalizou sua execucao.\n", currentTime, currentProcessId);
        processToExec--;
        currentProcessId = -1;
      }
      else {
        if(remainingSliceTime - 1 == 0) { //verifica se a fatia de tempo para o processo acabou
          enqueue(low, currentProcessId);
          printf("t = %d: Processo %d sofreu preempcao, inserido na fila de baixa prioridade.\n", currentTime, currentProcessId);
          currentProcessId = -1;
        }
        else {
          remainingSliceTime = remainingSliceTime - 1;
        }
      }
    }
  }
}

int main() {
  fp = fopen("tabela.csv", "w+");
  srand(time(0));
  init();

  runScheduler();

  for(int i = 0; i <= Nprocess; i++) {
    for(int j = 0; j <= timeTabela; j++) {
	    //printf("%s", tabela[i][j]);
	    fprintf(fp, "%s", tabela[i][j]);
	}
	//printf("\n");
	fprintf(fp, "\n");
  }
  fclose(fp);
  return 0;
}
