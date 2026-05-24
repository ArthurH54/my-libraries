#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"


typedef struct __Node {
    struct __Node* prev;
    struct __Node* next;
    void* data;
    size_t size;
    LinkedList container;
} _Node;

/**
 * @brief Creates a node through the copy of an element
 * @param element Pointer to the element whose content will be stored
 * @param size The size of the element in bytes
 * @return Pointer to the created node, or NULL on failure
 */
Node node_create(const void* element, size_t size) {
    if(element == NULL) return NULL;
    if(size == 0) return NULL;

    Node node = calloc(1, sizeof(_Node));
    if(node == NULL) return NULL;

    node->data = malloc(size); 
    if(node->data == NULL) { free(node); return NULL; }     

    memcpy(node->data, element, size);
    node->size = size;

    return node;
}

/** 
 * @brief Link two nodes
 * @param left The node on the left
 * @param right The node on the right
 * @return A constant indicating success or error
*/
StructureResult node_link(Node left, Node right) {
    if (left != NULL)  left->next = right;
    if(right != NULL)  right->prev = left;
    
    return STRUCTURE_OK;
}

/** 
 * @brief Detach a node from its neighbors
 * @param node Node to be unlinked
 * @warning Node must have a previous or posterior node
 * @return A constant indicating success or error
*/
StructureResult node_unlink(Node node) {
    if(node == NULL) return STRUCTURE_NULL_NODE;
    else {
        StructureResult state = node_link(node->prev, node->next);
        if(state != STRUCTURE_OK) return state;
        node->prev = NULL;
        node->next = NULL;
        return state;
    }
}

/**
 * @brief Destroys an unlinked node
 * @param node The node to be deallocated
 * @return A constant indicating success or error
 */
StructureResult node_destroy(Node* node) {
    if((*node) == NULL) return STRUCTURE_NULL_NODE;
    if((*node)->container != NULL) return STRUCTURE_CONTAINED_NODES;

    free((*node)->data);
    free((*node));
    (*node) = NULL;

    return STRUCTURE_OK;
}

/**
 * @brief Returns the data of the node
 * @param node The node of the wanted data
 * @return The data of the node
 */
const void* node_data(Node node) {
    return node->data;
}

/**
 * @brief Returns the size of the node
 * @param node The node of the wanted size
 * @return The size of the node
 */
size_t node_size(Node node) {
    return node->size;
}


//////////////////


typedef struct __LinkedList {
    Node start;
    Node end;
    size_t length;
    size_t size;
} _LinkedList;

/**
 * @brief Initializes the linked list
 * @returns Linked list
 */
static LinkedList list_create() { 
    LinkedList list = calloc(1, sizeof(_LinkedList));
    return list;
}

/** 
 * @brief Insert a node to the list
 * @param list The list to insert the node
 * @param node The node to be inserted
 * @param index The position where the node will be inserted, from 0 to length
 * @return A constant indicating success or error
*/
static StructureResult list_insert(LinkedList list, Node node, size_t index) {
    if(list == NULL) return STRUCTURE_NOT_CREATED;
    if(node == NULL) return STRUCTURE_NULL_NODE;
    if(index > list->length) return STRUCTURE_INVALID_ACCESS;
    if(node->container != NULL) return STRUCTURE_CONTAINED_NODES;

    if(list->length > 0) {
        if(index == 0) {
            node_link(node, list->start);
            list->start = node;
        }
        else if(index == list->length) {
            node_link(list->end, node);
            list->end = node;
        }
        else {
            Node left_node = list->start;
            for(size_t i = 0; i < index - 1; i++) left_node = left_node->next;
            Node right_node = left_node->next;
            node_link(node, right_node);
            node_link(left_node, node);
        }
    }
    else {
        list->start = node;
        list->end = node;
    }

    node->container = list;

    list->size += node->size;
    list->length++;
    
    return STRUCTURE_OK;
}

/** 
 * @brief Detaches a node from its list
 * @param list The list whose node will be removed
 * @param index The index of the node
 * @return A constant indicating success or error
*/
static Node list_detach(LinkedList list, size_t index) {
    if(list == NULL) return NULL;
    if(index >= list->length) return NULL;

    Node node = list->start;
    for(size_t i = 0; i < index; i++) node = node->next;
    Node previous = node->prev;
    Node next = node->next;

    list->size -= node->size;
    node_unlink(node);

    node->container = NULL;

    if(previous == NULL) list->start = next;
    if(next == NULL) list->end = previous;
    
    list->length--;
    return node;
}

/** 
 * @brief Removes all elements of the list
 * @param list The list to be emptied
 * @return A constant indicating success or error
*/
static StructureResult list_clear(LinkedList list) {
    if(list == NULL) return STRUCTURE_NOT_CREATED;
    if(list->length == 0) return STRUCTURE_EMPTY;

    // Pops list until it's empty
    while(list->length != 0) {
        Node node = list_detach(list, 0);
        StructureResult state = node_destroy(&node);
        if(state != STRUCTURE_OK) return state;
    }
    return STRUCTURE_OK;
}

/**
 * @brief Destroys the list, if it's not already destroyed
 * @param list The list to be destroyed
 * @return A constant indicating success or error
 */
static StructureResult list_destroy(LinkedList list) {
    StructureResult state = STRUCTURE_OK; 
    if(list != NULL) {
        if(list->length != 0) state = list_clear(list);
        free(list); 
        return state;
    }
    else return STRUCTURE_NOT_CREATED;
}

/**
 * @brief Returns the content of a member of the list
 * @param list The list whose member will be gotten
 * @param index The index of the member
 * @returns The content of the member
 */
static const void* list_get(const LinkedList list, size_t index) {
    if(list == NULL || index >= list->length) return NULL;

    Node position = list->start;
    for(size_t i = 0; i < index; i++) position = position->next;
    return position->data;
}

/**
 * @brief Returns the length of the list
 * @param list The list whose size will be gotten
 * @returns list length
 */
static size_t list_length(const LinkedList list) {
    if(list == NULL) return 0; 
    else return list->length;
}

/** 
 * @brief Returns the total size of the content of the nodes
 * @param list The list whose size will be gotten
 * @return The size of the list, or 0 if it's not created
*/
static size_t list_size(const LinkedList list) {
    if(list == NULL) return 0;
    else return list->size;
}

/** 
 * @brief Returns the memory occupied by the list
 * @param list The list whose occupied memory will be gotten
 * @return The memory occupied by the list
*/
static size_t list_memory(const LinkedList list) {
    if(list == NULL) return 0;
    else return sizeof(_LinkedList)          + 
                list->length * sizeof(_Node) + 
                list->size;
}


//////////////////


Stack stack_create() { return list_create(); }

StructureResult stack_push(Stack stack, Node node) { return list_insert(stack, node, list_length(stack)); }

Node stack_pop(Stack stack) { return list_detach(stack, list_length(stack)-1); }

StructureResult stack_clear(Stack stack) { return list_clear(stack); }

StructureResult stack_destroy(Stack stack) { return list_destroy(stack); }

const void* stack_peek(const Stack stack) { return list_get(stack, list_length(stack)-1); }

size_t stack_length(const Stack stack) { return list_length(stack); }

size_t stack_size(const Stack stack) { return list_size(stack); }

size_t stack_memory(const Stack stack) { return list_memory(stack); }


//////////////////


Queue queue_create() { return list_create(); }

StructureResult queue_enqueue(Queue queue, Node node) { return list_insert(queue, node, list_length(queue)); }

Node queue_dequeue(Queue queue) { return list_detach(queue, 0); }

StructureResult queue_clear(Queue queue) { return list_clear(queue); }

StructureResult queue_destroy(Queue queue) { return list_destroy(queue); }

const void* queue_head(const Queue queue) { return list_get(queue, 0); }

const void* queue_tail(const Queue queue) { return list_get(queue, list_length(queue)-1); }

size_t queue_length(const Queue queue) { return list_length(queue); }

size_t queue_size(const Queue queue) { return list_size(queue); }

size_t queue_memory(const Queue queue) { return list_memory(queue); }


//////////////////


Deque deque_create() { return list_create(); }

StructureResult deque_insert_front(Deque deque, Node node) { return list_insert(deque, node, 0); }

StructureResult deque_insert_back(Deque deque, Node node) { return list_insert(deque, node, list_length(deque)); }

Node deque_detach_front(Deque deque) { return list_detach(deque, 0); }

Node deque_detach_back(Deque deque) { return list_detach(deque, list_length(deque)-1); }

StructureResult deque_clear(Deque deque) { return list_clear(deque); }

StructureResult deque_destroy(Deque deque) { return list_destroy(deque); }

const void* deque_front(const Deque deque) { return list_get(deque, 0); }

const void* deque_back(const Deque deque) { return list_get(deque, list_length(deque)-1); }

size_t deque_length(const Deque deque) { return list_length(deque); }

size_t deque_size(const Deque deque) { return list_size(deque); }

size_t deque_memory(const Deque deque) { return list_memory(deque); }


//////////////////