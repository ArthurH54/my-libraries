// @return A constant indicating success or error
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"

/**
 * @brief Helper to verify the conditions of "insert" functions
 * @param position Pointer for the position in which a new node will be inserted
 * @param element The element whose content will be stored
 * @param size The size of the element in bytes
 * @return A constant indicating success or error
 */
static StructureResult structure_verify_insert_param(const void* structure, const void* element, size_t size) {
    if(structure == NULL) return STRUCTURE_NOT_CREATED;
    if(element == NULL) return STRUCTURE_NULL_ELEMENT;
    if(size == 0) return STRUCTURE_INVALID_SIZE; 
    else return STRUCTURE_OK;
}

//////////////////

typedef struct __Node {
    void* data;
    struct __Node* prev;
    struct __Node* next;
} _Node;
typedef _Node* Node;

/**
 * @brief Helper for the creation of a node through the copy of an element
 * @param element The element whose content will be stored
 * @param size The size of the element in bytes
 * @return Pointer to the created node, or NULL on failure
 */
static Node node_create(const void* element, size_t size) {
     // Creates the node to be pushed and verifies if it's correctly initialized
    Node newNode = calloc(1, sizeof(_Node));
    /* if error */ if(newNode == NULL) return NULL;
    newNode->data = malloc(size); 
    /* if error */ if(newNode->data == NULL) { free(newNode); return NULL; }                                 
    memcpy(newNode->data, element, size);
    return newNode;
}

/**
 * @brief Helper for the deallocation of a node
 * @param node The node to be deallocated
 */
static void node_destroy(Node* node) {
    free((*node)->data);
    free((*node));
    (*node) = NULL;
}

/** 
 * @brief Helper for the destruction of a node and rewind to the previous one
 * @param node Pointer to the node to be destroyed
 * @warning Requires the existence of a previous node
*/
static void node_destroy_and_move(Node* node) {
    // Node to be removed from the end  
    if((*node)->next == NULL) {
        Node prevNode;                 
        prevNode = (*node)->prev;        
        node_destroy(node);
        (*node) = prevNode;
    }

    // Node to be removed from the start
    else if((*node)->prev == NULL) {
        Node nextNode;                 
        nextNode = (*node)->next;        
        node_destroy(node);
        (*node) = nextNode;
    }
}

/** 
 * @brief Helper for the creation of a node and the demotion of its previous
 * @param position Pointer for the position in which a new node will be inserted
 * @param element The element whose content will be stored
 * @param size The size of the element in bytes
 * @warning Requires a structure with a valid existing node
 * @return A constant indicating success or error
*/
static StructureResult node_append(Node* position, const void* element, size_t size) {
    (*position)->next = node_create(element, size);
    if((*position)->next == NULL) return STRUCTURE_FAILED_ALLOCATION; 
    (*position)->next->prev = (*position);
    (*position) = (*position)->next;
    (*position)->next = NULL;

    return STRUCTURE_OK;
}

//////////////////

typedef struct {
    Node top;
    size_t length;
} _Stack;

/**
 * @brief Initializes the stack
 * @returns Stack
 */
Stack stack_create() { 
    Stack stack = calloc(1, sizeof(_Stack));
    return stack;
}

/**
 * @brief Pushes a new member onto the top of the stack
 * @param stack Pointer to the stack
 * @param element Pointer to the element to be copied
 * @param size The size of the element in bytes
 * @return A constant indicating success or error
 */
StructureResult stack_push(Stack stack, const void* element, size_t size) {
    StructureResult state = structure_verify_insert_param(stack, element, size);
    if(state != STRUCTURE_OK) return state;                                             

    // If stack is empty, pushes the node as the top member
    if(stack->top == NULL) {
        stack->top = node_create(element, size);
        if(stack->top == NULL) return STRUCTURE_FAILED_ALLOCATION;
    }
                             
    // If stack has elements, removes the previous node from the top and then pushes the new node
    else { 
        StructureResult state = node_append(&stack->top, element, size);   
        if(state == STRUCTURE_FAILED_ALLOCATION) return state;
    }                           

    stack->length++;
    return STRUCTURE_OK;
}

/**
 * @brief Removes the top of the stack, if the stack is not empty
 * @param stack Pointer to the stack
 * @return A constant indicating success or error
 */
StructureResult stack_pop(Stack stack) {
    if(stack == NULL) return STRUCTURE_NOT_CREATED;
    if(stack->top == NULL) return STRUCTURE_EMPTY;

    // If it's the last element 
    if(stack->length == 1) {
        node_destroy(&stack->top);
        stack->top = NULL;
        stack->length--;
    }

    // Saves the reference to the previous node, pops the top and assigns the previous node as the top
    else {
        node_destroy_and_move(&stack->top);
        stack->top->next = NULL;
        stack->length--;
    }

    return STRUCTURE_OK;
}

/** 
 * @brief Removes all elements of the stack
 * @param stack The stack to be emptied
 * @return A constant indicating success or error
*/
StructureResult stack_clear(Stack stack) {
    if(stack == NULL) return STRUCTURE_NOT_CREATED;
    if(stack->top == NULL) return STRUCTURE_EMPTY;

    // Pops stack until it's empty
    StructureResult state = STRUCTURE_OK;
    while(stack->length != 0) state = stack_pop(stack);
    return state;
}

/**
 * @brief Destroys the stack, if it's not already destroyed
 * @param stack The stack to be destroyed
 * @return A constant indicating success or error
 */
StructureResult stack_destroy(Stack stack) {
    StructureResult state = STRUCTURE_OK; 
    if(stack != NULL) {
        if(stack_length(stack) != 0) state = stack_clear(stack);
        free(stack); 
        return state;
    }
    else return STRUCTURE_NOT_CREATED;
}

/**
 * @brief Returns the content of the top of the stack
 * @param stack The stack whose top will be gotten
 * @returns The top member of the stack
 */
const void* stack_peek(const Stack stack) {
    if(stack == NULL || stack->top == NULL) return NULL;
    else return stack->top->data;
}

/**
 * @brief Returns the length of the stack
 * @param stack The stack whose size will be gotten
 * @returns Stack length
 */
size_t stack_length(const Stack stack) {
    if(stack == NULL) return 0; 
    else return stack->length;
}

//////////////////

typedef struct {
    Node head;
    Node tail;
    size_t length;
} _Queue;

/**
 * @brief Initializes the queue
 * @returns Queue
 */
Queue queue_create() {
    Queue queue = calloc(1, sizeof(_Queue));
    return queue;
}

/**
 * @brief Pushes a new member onto the tail of the queue
 * @param stack Pointer to the queue
 * @param element Pointer to the element to be copied
 * @param size The size of the element in bytes
 * @return A constant indicating success or error
 */
StructureResult queue_enqueue(Queue queue, const void* element, size_t size) {
    StructureResult state = structure_verify_insert_param(queue, element, size);
    if(state != STRUCTURE_OK) return state;                                                 

    // If queue is empty, pushes the node as the head and tail member
    if(queue->head == NULL) {                                                
        queue->head = node_create(element, size);
        queue->tail = queue->head;                       
        if(queue->head == NULL) return STRUCTURE_FAILED_ALLOCATION;       
    }

    // If queue has elements, removes the previous node from the tail and then pushes the new node
    else { 
        StructureResult state = node_append(&queue->tail, element, size);   
        if(state == STRUCTURE_FAILED_ALLOCATION) return state;
    }                     

    queue->length++;
    return STRUCTURE_OK;
}

/**
 * @brief Removes the first element of the queue, if the queue is not empty
 * @param queue Pointer to the queue
 * @return A constant indicating success or error
 */
StructureResult queue_dequeue(Queue queue) {
    if(queue == NULL) return STRUCTURE_NOT_CREATED;
    if(queue->head == NULL) return STRUCTURE_EMPTY;

    // If it's the last element 
    if(queue->length == 1) {
        node_destroy(&queue->head);
        queue->tail = NULL;
        queue->length--;
    }

    // Saves the reference to the next node, pops the head and assigns the previous node as the head
    else {
        node_destroy_and_move(&queue->head);
        queue->head->prev = NULL;
        queue->length--;
    }

    return STRUCTURE_OK;
}

/** 
 * @brief Removes all elements of the queue
 * @param queue The queue to be emptied
 * @return A constant indicating success or error
*/
StructureResult queue_clear(Queue queue) {
    if(queue == NULL) return STRUCTURE_NOT_CREATED;
    if(queue->head == NULL) return STRUCTURE_EMPTY;

    StructureResult state = STRUCTURE_OK;
    while(queue->length != 0) state = queue_dequeue(queue);
    return state;
}

/**
 * @brief Destroys the queue, if it's not already destroyed
 * @param queue The queue to be destroyed
 * @return A constant indicating success or error
 */
StructureResult queue_destroy(Queue queue) {
    StructureResult state = STRUCTURE_OK; 
    if(queue != NULL) {
        if(queue_length(queue) != 0) state = queue_clear(queue);
        free(queue); 
        return state;
    }
    else return STRUCTURE_NOT_CREATED;
}

/**
 * @brief Returns the content of the head of the queue
 * @param queue The queue whose head will be gotten
 * @returns const void *
 */
const void* queue_head(const Queue queue) {
    if(queue == NULL || queue->head == NULL) return NULL;
    else return queue->head->data;
}

/**
 * @brief Returns the content of the tail of the queue
 * @param queue The queue whose tail will be gotten
 * @returns const void *
 */
const void* queue_tail(const Queue queue) {
    if(queue == NULL || queue->tail == NULL) return NULL;
    else return queue->tail->data;
}

/**
 * @brief Returns the length of the queue
 * @param queue The queue whose size will be gotten
 * @returns int
 */
size_t queue_length(const Queue queue) {
    if(queue == NULL) return 0; 
    else return queue->length;
}

//////////////////