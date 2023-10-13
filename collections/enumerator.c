#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

#include "enumerator.h"

bool enumerator_enumerate_default(enumerator_t *enumerator, ...)
{
    va_list args;
    bool result;

    if (!enumerator->venumerate)
        return false;
    
    va_start(args, enumerator);
    result = enumerator->venumerate(enumerator, args);
    va_end(args);
    return result;
}

static bool enumerate_empty(enumerator_t *enumerator, va_list args)
{
    return false;
}

enumerator_t* enumerator_create_empty()
{
	enumerator_t *this;

    this = malloc(sizeof(*this));

    this->enumerate = enumerator_enumerate_default;
    this->venumerate = enumerate_empty;
    this->destroy = (void *)free;

	return this;
}