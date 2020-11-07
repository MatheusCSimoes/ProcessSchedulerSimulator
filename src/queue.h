#ifndef QUEUE_H
#define QUEUE_H

//A structure to represent a queue
typedef struct _Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
} Queue;

//Create a queue of given capacity.
Queue* createQueue(unsigned capacity);

//Check if the queue size is equal to the capacity
int isFull(Queue* queue);

//Check if the queue size is 0
int isEmpty(Queue* queue);

//Add an item to the queue
int enqueue(Queue* queue, int item);

//Remove the first item of the queue
int dequeue(Queue* queue);

//Function to get front of queue
int front(Queue* queue);

//Function to get rear of queue
int rear(Queue* queue);

#endif
