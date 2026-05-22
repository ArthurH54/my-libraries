#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"

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
 */
void stack_push(Stack stack, const void* element, size_t size) {
    if(stack == NULL || element == NULL || size == 0) return;                                               

    // Creates the node to be pushed and verifies if it's correctly initialized
    Node* newNode = (Node*) calloc(1, sizeof(Node));
    /* if error */ if(newNode == NULL) return;
    newNode->data = malloc(size); 
    /* if error */ if(newNode->data == NULL) { free(newNode); return; }                                 
    memcpy(newNode->data, element, size);

    // If stack is empty, pushes the node as the top member
    if(stack->top == NULL) {                                                                                                   
        stack->top = newNode;                         
    }

    // If stack has elements, removes the previous node from the top and then pushes the new node
    else {                                                                                                               
        stack->top->next = newNode;                                         
        stack->top->next->prev = stack->top;        
        stack->top = stack->top->next;                                      
        stack->top->next = NULL;                              
    }

    stack->length++;
}

/**
 * @brief Removes the top of the stack, if the stack is not empty
 * @param stack Pointer to the stack
 */
void stack_pop(Stack stack) {
    // Verifies if stack is uninitialized or empty
    if(stack == NULL || stack->top == NULL) return;

    // If it's the last element 
    if(stack->length == 1) {
        free(stack->top->data);
        free(stack->top);
        stack->top = NULL;
        stack->length--;
    }

    // Saves the reference to the previous node, pops the top and assigns the previous node as the top
    else {
        Node* previousNode;                 
        previousNode = stack->top->prev;        
        free(stack->top->data);
        free(stack->top);
        stack->top = previousNode;
        stack->top->next = NULL;

        stack->length--;
    }
}

/** 
 * @brief Removes all elements of the stack
 * @param stack The stack to be emptied
*/
void stack_clear(Stack stack) {
    // Verifies if the stack isn't already empty
    if(stack == NULL || stack->top == NULL) return;

    // Pops stack until it's empty
    while(stack->length != 0) stack_pop(stack);
}

/**
 * @brief Destroys the stack, if it's not already destroyed
 * @param stack The stack to be destroyed
 */
void stack_destroy(Stack stack) { 
    if(stack != NULL) {
        stack_clear(stack);
        free(stack); 
    }
}

/**
 * @brief Returns the content of the top of the stack
 * @param stack The stack whose top will be gotten
 * @returns const void *
 */
const void* stack_peek(const Stack stack) {
    if(stack == NULL || stack->top == NULL) return NULL;
    else return stack->top->data;
}

/**
 * @brief Returns the length of the stack
 * @param stack The stack whose size will be gotten
 * @returns int
 */
size_t stack_length(const Stack stack) {
    if(stack == NULL) return 0; 
    else return stack->length;
}

//////////////////

typedef struct {
    Node* head;
    Node* tail;
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
 */
void queue_enqueue(Queue queue, const void* element, size_t size) {
if(queue == NULL || element == NULL || size == 0) return;                                               

    // Creates the node to be pushed and verifies if it's correctly initialized
    Node* newNode = (Node*) calloc(1, sizeof(Node));
    /* if error */ if(newNode == NULL) return;

    // If queue is empty, pushes the node as the head and tail member
    if(queue->head == NULL) {                                                
        queue->head = newNode;                                               
        queue->head->data = malloc(size); 
        /* if error */ if(queue->head->data == NULL) { free(queue->head); return; }
        memcpy(queue->head->data, element, size); 
        queue->tail = queue->head;                              
    }

    // If queue has elements, removes the previous node from the tail and then pushes the new node
    else {                                                                  
        queue->tail->next = newNode;                                         
        queue->tail->next->prev = queue->tail;        
        queue->tail = queue->tail->next;                                      
        queue->tail->next = NULL;                                            
        queue->tail->data = malloc(size); 
        /* if error */ if(queue->tail->data == NULL) { free(queue->tail); return; }                                 
        memcpy(queue->tail->data, element, size);                               
    }

    queue->length++;
}

/**
 * @brief Removes the first element of the queue, if the queue is not empty
 * @param queue Pointer to the queue
 */
void queue_dequeue(Queue queue) {
    // Verifies if queue is uninitialized or empty
    if(queue == NULL || queue->head == NULL) return;

    // If it's the last element 
    if(queue->length == 1) {
        free(queue->head->data);
        free(queue->head);
        queue->head = NULL;
        queue->tail = NULL;
        queue->length--;
    }

    // Saves the reference to the next node, pops the head and assigns the previous node as the head
    else {
        Node* nextNode;                 
        nextNode = queue->head->next;        
        free(queue->head->data);
        free(queue->head);
        queue->head = nextNode;
        queue->head->prev = NULL;

        queue->length--;
    }
}

/** 
 * @brief Removes all elements of the queue
 * @param queue The queue to be emptied
*/
void queue_clear(Queue queue) {
    // Verifies if the queue isn't already empty
    if(queue == NULL || queue->head == NULL) return;

    // Pops stack until it's empty
    while(queue->length != 0) queue_dequeue(queue);
}

/**
 * @brief Destroys the queue, if it's not already destroyed
 * @param queue The queue to be destroyed
 */
void queue_destroy(Queue queue) {
    if(queue != NULL) {
        queue_clear(queue);
        free(queue); 
    }
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