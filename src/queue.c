#include "queue.h"

void init_queue(Queue* q) {
    q -> front = 0;
    q -> rear = 0;
    q -> lenght = SHAPE_QUEUE_LEN;
    q -> size = 0;
}

void free_queue(Queue* q) { 
    free(q);
}

bool is_empty(Queue* q) { 
    return q->size == 0;
}

bool is_full(Queue* q) {
    return q->size == q->lenght;
}

void enqueue(Queue* q, Shape shape) {
    // add an element to the back
    if (is_full(q))
    {
        printf("Queue is full\n");
        return;
    }
    q->shapes[q->rear] = shape;
    q -> rear = (++q->rear) % q->lenght;
    q->size++;
    return;
}

Shape dequeue(Queue* q) {
    // remove first element
    if (is_empty(q)) {
        printf("Queue is empty\n");
        return -1;
    }
    
    Shape shape = q->shapes[q->front];
    q->front = (++q->front) % q -> lenght;
    q->size--;
    return shape;
}

void print_queue(Queue* q) {
    if (is_empty(q))
    {
        printf("Queue is empty\n");
        return;
    }

    printf("Current Queue: ");
    for (int i = 0; i < q->size; i++)
    {
        int j = (q->front + i) % q->lenght;
        printf("%d ", q->shapes[j]);
    }
    printf("\n");
}