#ifndef PROCESS_H
#define PROCESS_H

struct Process* createProcess(int id, int execTime);
int remaining(struct Process* process);
int exec(struct Process* process, int execTime);

#endif
