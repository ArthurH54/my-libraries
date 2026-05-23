#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stddef.h>

typedef enum {
    STRUCTURE_OK = 0,
    STRUCTURE_FAILED_ALLOCATION,
    STRUCTURE_NOT_CREATED,
    STRUCTURE_EMPTY,
    STRUCTURE_NULL_ELEMENT,
    STRUCTURE_INVALID_SIZE
} StructureResult;

typedef _Stack* Stack;

Stack stack_create(void);
StructureResult stack_push(Stack stack, const void* element, size_t size);
StructureResult stack_pop(Stack stack);
StructureResult stack_clear(Stack stack);
StructureResult stack_destroy(Stack stack);
const void* stack_peek(const Stack stack);
size_t stack_length(const Stack stack);

//////////////////

typedef _Queue* Queue;

Queue queue_create();
StructureResult queue_enqueue(Queue queue, const void* element, size_t size);
StructureResult queue_dequeue(Queue queue);
StructureResult queue_clear(Queue queue);
StructureResult queue_destroy(Queue queue);
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
