#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include "collections/array.h"
#include "collections/enumerator.h"

typedef struct dbg_group
{
    char *name;
    int group_id;
}dbg_group_t;

dbg_group_t group_arr[] = 
{
    {.group_id = 0, .name = "group0"},
    {.group_id = 1, .name = "group1"},
    {.group_id = 2, .name = "group2"},
    {.group_id = 3, .name = "group3"},
    {.group_id = 4, .name = "group4"},
    {.group_id = 5, .name = "group5"},
    {.group_id = 6, .name = "group6"},
    {.group_id = 7, .name = "group7"},
};

static int compare(dbg_group_t *a, dbg_group_t *b, void *user)
{
    return b->group_id - a->group_id;
}

static int cmp(dbg_group_t *a, dbg_group_t *b)
{
    return compare(a, b, NULL);
}

void array_callback(dbg_group_t *entry, int idx, void *user)
{
    fprintf(stderr, "[%s] gid is %d, name is %s\n", 
                    (char *)user, entry->group_id, entry->name);
    return ;
}

int array_test()
{
    dbg_group_t instence, lookup;
    dbg_group_t *entry;
    array_t *groups = array_create(sizeof(dbg_group_t), 8);

    array_insert(groups, ARRAY_TAIL, &group_arr[0]);
    array_insert(groups, ARRAY_TAIL, &group_arr[1]);
    array_insert(groups, ARRAY_TAIL, &group_arr[2]);
    array_insert(groups, ARRAY_TAIL, &group_arr[3]);
    array_insert(groups, ARRAY_TAIL, &group_arr[4]);

    array_get(groups, 0, &instence);
    fprintf(stderr, "[%s] gid is %d, name is %s\n", "array_get", instence.group_id, instence.name);
    array_get(groups, -1, &instence);
    fprintf(stderr, "[%s] gid is %d, name is %s\n", "array_get", instence.group_id, instence.name);

    enumerator_t *e = array_create_enumerator(groups);

    while (e->enumerate(e, &entry))
        fprintf(stderr, "[%s] gid is %d, name is %s\n", "enumerate", 
                                        entry->group_id, entry->name);

    e->destroy(e);

    array_sort(groups, (int (*)(const void *, const void *, void *))compare, NULL);

    e = array_create_enumerator(groups);

    while (e->enumerate(e, &entry))
        fprintf(stderr, "[%s] gid is %d, name is %s\n", "array_sort", 
                                        entry->group_id, entry->name);
    
    e->destroy(e);
    lookup.group_id = 3;
    array_bsearch(groups, &lookup, 
                        (int (*)(const void *, const void *))cmp, &instence);
    fprintf(stderr, "[%s] gid is %d, name is %s\n", "array_bsearch", instence.group_id, instence.name);

    fprintf(stderr, "[%s] array size is %d\n", "array_count", array_count(groups));

    array_remove(groups, 2, &instence);
    fprintf(stderr, "[%s] gid is %d, name is %s\n", "array_remove", instence.group_id, instence.name);
    fprintf(stderr, "[%s] array size is %d\n", "array_remove", array_count(groups));

    array_invoke(groups, (array_callback_t)array_callback, "array_invoke");

    array_destroy(groups);

    return 0;
}