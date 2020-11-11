#ifndef PROCESS_H
#define PROCESS_H

//A structure to represent a process
typedef struct _Process {
  int id;
  int startTime; //apenas para agendar o inicio na simulacao
  int timeExecuted;
  int executionTime;
  int endTime;  //apenas para guardar o fim da simulação e calcular o turnaround
} Process;

Process* createProcess(int id, int execTime, int initTime);
int remaining(Process* process);
int exec(Process* process, int execTime);

#endif
