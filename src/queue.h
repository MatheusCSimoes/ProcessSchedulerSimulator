#ifndef QUEUE_H
#define QUEUE_H

//Create a queue of given capacity.
struct Queue* createQueue(unsigned capacity);

//Check if the queue size is equal to the capacity
int isFull(struct Queue* queue);

//Check if the queue size is 0
int isEmpty(struct Queue* queue);

//Add an item to the queue
void enqueue(struct Queue* queue, int item);

//Remove the first item of the queue
int dequeue(struct Queue* queue);

//Function to get front of queue
int front(struct Queue* queue);

//Function to get rear of queue
int rear(struct Queue* queue);

#endif
