#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"

/**
 * @brief Initializes the stack, if it's not already initialized
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

    // If stack is empty, pushes the node as the top member
    if(stack->top == NULL) {                                                
        stack->top = newNode;                                               
        stack->top->data = malloc(size); 
        /* if error */ if(stack->top->data == NULL) { free(stack->top); return; }
        memcpy(stack->top->data, element, size);                               
    }

    // If stack has elements, removes the previous node from the top and then pushes the new node
    else {                                                                  
        stack->top->next = newNode;                                         
        stack->top->next->prev = stack->top;        
        stack->top = stack->top->next;                                      
        stack->top->next = NULL;                                            
        stack->top->data = malloc(size); 
        /* if error */ if(stack->top->data == NULL) { free(stack->top); return; }                                 
        memcpy(stack->top->data, element, size);                               
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
void stack_empty(Stack stack) {
    // Verifies if the stack isn't already empty
    if(stack == NULL || stack->top == NULL) return;

    // Pops stack until it's empty
    while(stack->length != 0) popStack(stack);
}

/**
 * @brief Destroys the stack, if it's not already destroyed
 * @param stack The stack to be destroyed
 */
void stack_destroy(Stack stack) { 
    if(stack != NULL) {
        emptyStack(stack);
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
 * @brief Gets a member of the stack, counting from the top
 * @param stack The stack whose member will be gotten
 * @param depth The depth of the member in the stack
 * @returns const void *
*/
// const void* stack_item(const Stack stack, size_t depth) {
    if(stack == NULL) return NULL;

    // Verifies if the given depth is larger than the length
    if(depth >= stack->length) return NULL; 

    // Iterates until it finds the member of given depth
    Node* member = stack->top;
    for(size_t i = 0; i < depth; i++) member = member->prev;

    return member->data;
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

