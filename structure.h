#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stddef.h>

typedef _Stack* Stack;

Stack stack_create();
void stack_push(Stack stack, const void* element, size_t size);
void stack_pop(Stack stack);
void stack_clear(Stack stack);
void stack_destroy(Stack stack);
const void* stack_peek(const Stack stack);
size_t stack_length(const Stack stack);

//////////////////

typedef _Queue* Queue;

Queue queue_create();
void queue_enqueue(Queue queue, const void* element, size_t size);
void queue_dequeue(Queue queue);
void queue_clear(Queue queue);
void queue_destroy(Queue queue);
const void* queue_head(const Queue queue);
const void* queue_tail(const Queue queue);
size_t queue_length(const Queue queue);

//////////////////

/**
 * @note New implementations soon
 * @note Not tested
 */

//////////////////

#endif
