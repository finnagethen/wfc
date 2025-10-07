#include "list.h"
#include <assert.h>
#include <stdlib.h>

/**
 * @func list_alloc
 *
 * @brief Allocates space for one 'list_s' element. Usually called internally.
 *
 * @returns a pointer to the newly allocated 'list_s' element
 **/
list_s* list_alloc(void) {
    list_s* list = malloc(sizeof(list_s));
    assert(list);
    *list = (list_s){ 0 };
    return list;
}

/**
 * @func list_free1
 *
 * @brief Frees one 'list_s' element, but does not update nodes.
 *
 * @param list a 'list_s' element
 **/
void list_free1(list_s* list) { free(list); }

/**
 * @func list_free
 *
 * @brief Frees an entire linked list.
 *
 * @param list the head of a linked list
 **/
void list_free(list_s* list) {
    while (list) {
        list_s* next = list->next;
        free(list);
        list = next;
    }
}

/**
 * @func list_back
 *
 * @brief Iterates through a linked list to find the back.
 *
 * @param list a node in a linked list
 *
 * @returns the back of the linked list, or NULL if the list is empty
 **/
list_s* list_back(list_s* list) {
    if (!list)
        return NULL;

    list_s* curr = list;
    while (curr->next) {
        curr = curr->next;
    }
    return curr;
}

/**
 * @func list_front
 *
 * @brief Iterates through a linked list to find the front.
 *
 * @param list a node in a linked list
 *
 * @returns the front of the linked list, or NULL if the list is empty
 **/
list_s* list_front(list_s* list) {
    if (!list)
        return NULL;

    list_s* curr = list;
    while (curr->prev) {
        curr = curr->prev;
    }
    return curr;
}

/**
 * @func list_push_back
 *
 * @brief Appends a new element to the end of the linked list.
 *
 * @param list the head of a linked list
 * @param data pointer to the data to store in the new element
 *
 * @returns the head of the list (maybe new if the original list was empty)
 **/
list_s* list_push_back(list_s* list, void* data) {
    list_s* new_list = list_alloc();
    new_list->data = data;
    new_list->next = NULL;
    new_list->prev = NULL;

    if (list) {
        list_s* tail = list_back(list);
        tail->next = new_list;
        new_list->prev = tail;

        return list;
    }

    return new_list;
}

/**
 * @func list_push_front
 *
 * @brief Prepends a new element to the front of the linked list.
 *
 * @param list the head of a linked list
 * @param data pointer to the data to store in the new element
 *
 * @returns the new head of the list
 **/
list_s* list_push_front(list_s* list, void* data) {
    list_s* new_list = list_alloc();
    new_list->data = data;
    new_list->next = list;
    new_list->prev = NULL;

    if (list) {
        list->prev = new_list;
    }

    return new_list;
}

/**
 * @func list_remove_link
 *
 * @brief Removes an element from the linked list without freeing it.
 * The removed element's 'next' and 'prev' pointers are set to NULL.
 *
 * @param list the head of a linked list
 * @param target the element to remove from the list
 *
 * @returns the head of the list
 **/
list_s* list_remove_link(list_s* list, list_s* target) {
    if (!target)
        return list;

    if (target->prev) {
        target->prev->next = target->next;
    }
    if (target->next) {
        target->next->prev = target->prev;
    }
    if (list == target) {
        list = list->next;
    }

    target->next = NULL;
    target->prev = NULL;

    return list;
}

/**
 * @func list_pop_back
 *
 * @brief Removes the last element of the list with freeing it.
 *
 * @param list the head of a linked list
 *
 * @retuns the possibly changed head of the list
 **/
list_s* list_pop_back(list_s* list) {
    if (!list)
        return NULL;

    list_s* back = list_back(list);
    list = list_remove_link(list, back);
    list_free1(back);
    return list;
}

/**
 * @func list_pop_front
 *
 * @brief Removes the last element of the list with freeing it.
 *
 * @param list the head of a linked list
 *
 * @returns the possibly changed head of the list
 **/
list_s* list_pop_front(list_s* list) {
    if (!list)
        return NULL;

    list_s* front = list_front(list);
    list = list_remove_link(list, front);
    list_free1(front);
    return list;
}

/**
 * @func list_foreach
 *
 * @brief Iterates through the list and calls the given function for each
 *element.
 *
 * @param list the head of a linked list
 * @param func the function to call for each element
 * @param context a context pointer to pass to the function
 **/
void list_foreach(list_s* list, list_func func, void* context) {
    list_s* curr = list;
    while (curr) {
        func(curr->data, context);
        curr = curr->next;
    }
}
