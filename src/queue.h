#ifndef QUEUE_H
#define QUEUE_H

//A structure to represent a queue
typedef struct _Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
} Queue;

Queue* createQueue(unsigned capacity);
int isFull(Queue* queue);
int isEmpty(Queue* queue);
int enqueue(Queue* queue, int item);
int dequeue(Queue* queue);
int front(Queue* queue);
int rear(Queue* queue);

#endif
