#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

Queue* createQueue(unsigned capacity)
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    queue->rear = capacity - 1;
    queue->array = (int*)malloc(
        queue->capacity * sizeof(int));
    return queue;
}

int isFull(Queue* queue) {
    return (queue->size == queue->capacity);
}

int isEmpty(Queue* queue) {
    return (queue->size == 0);
}

int enqueue(Queue* queue, int item) {
    if (isFull(queue)) {
        return 0; //false
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    return 1; //true
}

int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        return -1;
    }
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

int front(Queue* queue) {
    if (isEmpty(queue)) {
      return -1;
    }
    return queue->array[queue->front];
}

int rear(Queue* queue) {
    if (isEmpty(queue)) {
        return -1;
    }
    return queue->array[queue->rear];
}
