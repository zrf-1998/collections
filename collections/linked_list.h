#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include "enumerator.h"

typedef struct linked_list_t linked_list_t;

typedef void (*linked_list_invoke_t)(void *item, va_list args);
typedef bool (*linked_list_match_t)(void *item, va_list args);

struct linked_list_t
{
    int (*get_count) (linked_list_t *this);
    enumerator_t* (*create_enumerator)(linked_list_t *this);
    void (*reset_enumerator)(linked_list_t *this, enumerator_t *enumerator);
    status_t (*get_first) (linked_list_t *this, void **item);
    status_t (*get_last) (linked_list_t *this, void **item);
    bool (*find_first)(linked_list_t *this, linked_list_match_t match, void **item, ...);
    void (*insert_first) (linked_list_t *this, void *item);
    void (*insert_last) (linked_list_t *this, void *item);
    void (*insert_before)(linked_list_t *this, enumerator_t *enumerator, void *item);
    status_t (*remove_first) (linked_list_t *this, void **item);
    status_t (*remove_last) (linked_list_t *this, void **item);
    int (*remove)(linked_list_t *this, void *item, bool (*compare)(void*,void*));
    void (*remove_at)(linked_list_t *this, enumerator_t *enumerator);
    void (*invoke_offset)(linked_list_t *this, size_t offset);
    linked_list_t *(*clone_offset) (linked_list_t *this, size_t offset);

    void (*invoke_function)(linked_list_t *this, linked_list_invoke_t function, ...);
    void (*destroy) (linked_list_t *this);
};

linked_list_t *linked_list_create();
#endif
