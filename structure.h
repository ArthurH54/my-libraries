// Includes functions for stacks, queues and deques
#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stddef.h>


typedef enum {
    STRUCTURE_OK = 0,
    STRUCTURE_FAILED_ALLOCATION,
    STRUCTURE_NOT_CREATED,
    STRUCTURE_EMPTY,
    STRUCTURE_INVALID_ACCESS,
    STRUCTURE_NULL_NODE,
    STRUCTURE_CONTAINED_NODES,
} StructureResult;



//////////////////


typedef struct __Node _Node;
typedef _Node* Node;

Node node_create(const void* element, size_t size);
StructureResult node_link(Node left, Node right);
StructureResult node_unlink(Node node);
StructureResult node_destroy(Node* node);


//////////////////


typedef struct __LinkedList _LinkedList;
typedef _LinkedList* LinkedList;


//////////////////


typedef LinkedList Stack;

Stack stack_create();
StructureResult stack_push(Stack stack, Node node);
Node stack_pop(Stack stack);
StructureResult stack_clear(Stack stack);
StructureResult stack_destroy(Stack stack);
const void* stack_peek(const Stack stack);
size_t stack_length(const Stack stack);
size_t stack_size(const Stack stack);
size_t stack_memory(const Stack stack);


//////////////////


typedef LinkedList Queue;

Queue queue_create();
StructureResult queue_enqueue(Queue queue, Node node);
Node queue_dequeue(Queue queue);
StructureResult queue_clear(Queue queue);
StructureResult queue_destroy(Queue queue);
const void* queue_head(const Queue queue);
const void* queue_tail(const Queue queue);
size_t queue_length(const Queue queue);
size_t queue_size(const Queue queue);
size_t queue_memory(const Queue queue);


//////////////////


typedef LinkedList Deque;

Deque deque_create();
StructureResult deque_insert_front(Deque deque, Node node);
StructureResult deque_insert_back(Deque deque, Node node);
Node deque_detach_front(Deque deque);
Node deque_detach_back(Deque deque);
StructureResult deque_clear(Deque deque);
StructureResult deque_destroy(Deque deque);
const void* deque_front(const Deque deque);
const void* deque_back(const Deque deque);
size_t deque_length(const Deque deque);
size_t deque_size(const Deque deque);
size_t deque_memory(const Deque deque);


//////////////////





//////////////////

/**
 * @note New implementations soon
 * @note Not tested
 */

//////////////////

#endif