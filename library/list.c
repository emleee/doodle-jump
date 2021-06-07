#include <stdlib.h>
#include <assert.h>
#include "list.h"

typedef struct list {
    void **data;
    size_t size;
    size_t capacity;
    free_func_t freer;
} list_t;

list_t *list_init(size_t initial_size, free_func_t freer) {
    list_t *list = malloc(sizeof(list_t));
    assert(list != NULL);

    list->data = malloc(initial_size * sizeof(void *));
    list->capacity = initial_size;
    list->size = 0;
    list->freer = freer;

    return list;
}

void *list_get(list_t *list, size_t index) {
    assert(index < list->size);
    return list->data[index];
}

size_t list_size(list_t *list) {
    return list->size;
}

free_func_t get_freer(list_t*list)
{
    return list->freer;
}

void list_add(list_t *list, void *value){
    if (list->size == list->capacity) {
        list_resize(list);
    }
    assert(list->size < list->capacity);
    assert(value != NULL);

    list->data[list->size] = value;
    list->size++;
}

void *list_remove(list_t *list, size_t index) {
    assert(list->size != 0);

    void *data = list->data[index];
    for (size_t j = index; j < list->size - 1; j++) {
        list->data[j] = list->data[j+1];
    }
    list->size--;

    return data;
}

 void list_resize(list_t *list) {
    assert(list->size == list->capacity);
    
    void **bigger = malloc((list_size(list) * 2 + 1) * sizeof(void *));
    for(int i = 0; i < list->size; i++)
    {
        bigger[i] = list_get(list, i);
    }
    free(list->data);
    
    list->capacity = list_size(list) * 2 + 1;
    list->data = bigger;
}

void list_free(list_t *list) {
    for (size_t i = 0; i < list->size; i++) {
        if (list->freer != NULL) {
            list->freer(list->data[i]);
        }
    }
    free(list->data);
    free(list);
}