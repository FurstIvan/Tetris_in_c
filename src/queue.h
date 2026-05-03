#ifndef TETRIS_QUEUE_H
#define TETRIS_QUEUE_H

#include "types.h"

void init_queue(Queue*);
bool is_empty(Queue*);
bool is_full(Queue*);
void enqueue(Queue*, Shape);
Shape dequeue(Queue*);
void print_queue(Queue*);
void free_queue(Queue*);

#endif