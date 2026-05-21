#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stddef.h>

typedef struct _Node {
    void* data;
    struct _Node* prev;
    struct _Node* next;
} Node;

//////////////////

typedef struct {
    Node* top;
    size_t length;
} _Stack;
typedef _Stack* Stack;

Stack stack_create();
void stack_push(Stack stack, const void* element, size_t size);
void stack_pop(Stack stack);
void stack_empty(Stack stack);
void stack_destroy(Stack stack);
const void* stack_peek(const Stack stack);
// const void* stack_item(const Stack stack, size_t depth)
size_t stack_length(const Stack stack);

//////////////////

typedef struct {
    Node* front;
    Node* back;
    size_t length;
} _Queue;
typedef _Queue* Queue;

Queue queue_create();
void queue_enqueue(Queue queue, const void* element, size_t size);
void queue_dequeue(Queue queue);
void queue_empty(Queue queue);
void queue_destroy(Queue queue);
const void* queue_front(const Queue queue);
const void* queue_back(const Queue queue);
// const void* queue_item(const Queue queue, size_t depth)
size_t queue_length(const Queue queue);

//////////////////

/**
 * @note New implementations soon
 */

//////////////////

#endif
