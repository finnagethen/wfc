#pragma once

typedef struct list {
    struct list* next;
    struct list* prev;
    void* data;
} list_s;

typedef void (*list_func)(void* data, void* context);

list_s* list_alloc(void);

void list_free1(list_s* list);

void list_free(list_s* list);

list_s* list_back(list_s* list);

list_s* list_front(list_s* list);

list_s* list_push_back(list_s* list, void* data);

list_s* list_push_front(list_s* list, void* data);

list_s* list_remove_link(list_s* list, list_s* target);

list_s* list_pop_back(list_s* list);

list_s* list_pop_front(list_s* list);

void list_foreach(list_s* list, list_func func, void* context);
