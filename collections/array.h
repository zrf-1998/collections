#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "utils/utils.h"
#include "enumerator.h"

/** maximum number of unused head/tail elements before cleanup */
#define ARRAY_MAX_UNUSED 32

enum array_idx_t {
	ARRAY_HEAD = 0,
	ARRAY_TAIL = -1,
};

typedef struct array_t array_t;
typedef void (*array_callback_t)(void *data, int idx, void *user);

array_t *array_create(u_int esize, u_int8_t reserve);
int array_count(array_t *array);
void array_compress(array_t *array);
enumerator_t* array_create_enumerator(array_t *array);
void array_insert(array_t *array, int idx, void *data);
void array_insert_create(array_t **array, int idx, void *ptr);
void array_insert_create_value(array_t **array, u_int esize,
							   int idx, void *val);
void array_insert_enumerator(array_t *array, int idx, enumerator_t *enumerator);
bool array_get(array_t *array, int idx, void *data);
bool array_remove(array_t *array, int idx, void *data);
void array_sort(array_t *array, int (*cmp)(const void*,const void*,void*),
				void *user);
int array_bsearch(array_t *array, const void *key,
				  int (*cmp)(const void*,const void*), void *out);
void array_invoke(array_t *array, array_callback_t cb, void *user);
void array_destroy(array_t *array);

#endif