#include <stdlib.h>
#include <stdio.h>

//A structure to represent a process
struct Process {
  int id;
  int executed;
  int executionTime;
};

struct Process* createProcess(unsigned execTime)
{
  struct Process* process = (struct Process*)malloc(sizeof(struct Process));

  process->id = 0; //calcular id
  process->executed = 0;
  process->executionTime = execTime;

  return process;
}

int remaining(struct Process* process)
{
  return (process->executionTime - process->executed);
}

//retorna o tempo não utilizado
int exec(struct Process* process, int execTime)
{
  int remainingTime = remaining(process);
  if(execTime > remainingTime) {
      process->executed = process->executionTime;
      return (execTime - remainingTime);
  }

  process->executed = process->executed + execTime;
  return 0;
}
