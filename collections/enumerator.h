#ifndef __ENUMERATOR_H__
#define __ENUMERATOR_H__

#include "utils/utils.h"

typedef struct enumerator_t enumerator_t;

struct enumerator_t
{
    bool (*enumerate)(enumerator_t *this, ...);
	bool (*venumerate)(enumerator_t *this, va_list args);
	void (*destroy)(enumerator_t *this);
};

bool enumerator_enumerate_default(enumerator_t *enumerator, ...);
enumerator_t* enumerator_create_empty();
#endif
