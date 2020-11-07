#include <stdlib.h>
#include <stdio.h>

//A structure to represent a process
struct Process {
  int id;
  int timeExecuted;
  int executionTime;
};

struct Process* createProcess(int id, int execTime) {
  struct Process* process = (struct Process*)malloc(sizeof(struct Process));

  process->id = id;
  process->timeExecuted = 0;
  process->executionTime = execTime;

  return process;
}

int remaining(struct Process* process) {
  return (process->executionTime - process->timeExecuted);
}

//retorna o tempo não utilizado
int exec(struct Process* process, int execTime) {
  int remainingTime = remaining(process);
  if(execTime > remainingTime) {
      process->timeExecuted = process->executionTime;
      return (execTime - remainingTime);
  }

  process->timeExecuted = process->timeExecuted + execTime;
  return 0;
}
