#include <stdlib.h>
#include <stdio.h>

#include "process.h"

Process* createProcess(int id, int execTime, int initTime) {
  Process* process = (Process*)malloc(sizeof(Process));

  process->id = id;
  process->timeExecuted = 0;
  process->executionTime = execTime;
  process->startTime = initTime;

  return process;
}

int remaining(Process* process) {
  return (process->executionTime - process->timeExecuted);
}

//retorna o tempo nao utilizado
int exec(Process* process, int execTime) {
  int remainingTime = remaining(process);
  if(execTime > remainingTime) {
      process->timeExecuted = process->executionTime;
      return (execTime - remainingTime);
  }

  process->timeExecuted = process->timeExecuted + execTime;
  return 0;
}
