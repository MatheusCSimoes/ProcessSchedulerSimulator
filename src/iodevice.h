#ifndef IODEVICE_H
#define IODEVICE_H

struct IOdevice* createIOdevice(unsigned execTime, int maxProcesses);
int enqueueIO(struct IOdevice* device, unsigned pid);
int remainingIO(struct IOdevice* device);
int execIO(struct IOdevice* device, int execTime);

#endif
