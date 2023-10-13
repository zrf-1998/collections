#include <stdarg.h>
#include <stdlib.h>

#include "utils/utils.h"
#include "linked_list.h"
#include "collections/enumerator.h"

typedef struct element_t element_t;

struct element_t
{
    void *value;
    element_t *prev;
    element_t *next;
};

static element_t *element_create(void *value)
{
    element_t *this;
    this = calloc(1, sizeof(*this));
    this->value = value;
    return this;
}

typedef struct private_linked_list_t private_linked_list_t;

struct private_linked_list_t
{
    linked_list_t public;
    int count;
    element_t *first;
    element_t *last;
};

typedef struct private_enumerator_t private_enumerator_t;

struct private_enumerator_t
{
    enumerator_t public;
    private_linked_list_t *list;
    element_t *current;
};

static bool do_enumerate(private_enumerator_t *this, va_list args)
{
	void **item;

	VA_ARGS_VGET(args, item);

	if (!this->current)
	{
		return false;
	}
	if (item)
	{
		*item = this->current->value;
	}
	return true;
}

static bool enumerate_next(enumerator_t *this, va_list args)
{
    private_enumerator_t *private = (private_enumerator_t *)this;
    if (private->current)
    {
        private->current = private->current->next;
    }
    return do_enumerate(private, args);
}

static bool enumerate_current(enumerator_t *this, va_list args)
{
    private_enumerator_t *private = (private_enumerator_t *)this;
    private->public.venumerate = enumerate_next;
    return do_enumerate(private, args);
}

static enumerator_t *create_enumerator(linked_list_t *this)
{
    private_linked_list_t *private = (private_linked_list_t *)this;
    private_enumerator_t *enumerator;
    enumerator = calloc(1, sizeof(*enumerator));

    enumerator->list = private;
    enumerator->current = private->first;

    enumerator->public.enumerate = enumerator_enumerate_default;
    enumerator->public.venumerate = enumerate_current;
    enumerator->public.destroy = (void *)free;
    
    return &enumerator->public;
}

static void reset_enumerator(linked_list_t *this, enumerator_t *enumerate)
{
    private_enumerator_t *private_e = (private_enumerator_t *)enumerate;
    private_linked_list_t *private_l = (private_linked_list_t *)this;

    private_e->current = private_l->first;
    private_e->public.venumerate = enumerate_current;
    return ;
}

static int get_count(linked_list_t *public)
{
    private_linked_list_t *this = (private_linked_list_t *)public;
    return this->count;
}

static void insert_first(linked_list_t *public, void *item)
{
    private_linked_list_t *this = (private_linked_list_t *)public;
    element_t *elem;

    elem = element_create(item);
    if (this->count == 0)
    {
        this->first = elem;
        this->last = elem;
    }
    else 
    {
        elem->next = this->first;
        this->first->prev = elem;
        this->first = elem;
    }
    this->count++;

    return ;
}

static void insert_last(linked_list_t *public, void *item)
{
    private_linked_list_t *this = (private_linked_list_t *)public;
    element_t *elem;

    elem = element_create(item);
    if (this->count == 0)
    {
        this->first = elem;
        this->last = elem;
    }
    else 
    {
        elem->prev = this->last;
        this->last->next = elem;
        this->last = elem;
    }
    this->count++;

    return ;
}

static void invoke_function(linked_list_t *public, linked_list_invoke_t fn, ...)
{
    private_linked_list_t *this = (private_linked_list_t *)public;
    element_t *current = this->first;
    va_list args;

    while (current)
    {
        va_start(args, fn);
        fn(current->value, args);
        va_end(args);
        current = current->next;
    }

    return ;
}

static element_t* remove_element(private_linked_list_t *this,
								 element_t *element)
{
	element_t *next, *previous;

	next = element->next;
	previous = element->prev;
	free(element);
	if (next)
	{
		next->prev = previous;
	}
	else
	{
		this->last = previous;
	}
	if (previous)
	{
		previous->next = next;
	}
	else
	{
		this->first = next;
	}
	if (--this->count == 0)
	{
		this->first = NULL;
		this->last = NULL;
	}
	return next;
}

static status_t get_first(linked_list_t *public, void **item)
{
    private_linked_list_t *this = (private_linked_list_t *)public;
    if (this->count == 0)
    {
        return NOT_FOUND;
    }
    *item = this->first->value;
    return SUCCESS;
}

static status_t get_last(linked_list_t *public, void **item)
{
    private_linked_list_t *this = (private_linked_list_t *)public;
    if (this->count == 0)
    {
        return NOT_FOUND;
    }
    *item = this->last->value;
    return SUCCESS;
}

static status_t remove_first(linked_list_t *public, void **item)
{
    private_linked_list_t *this = (private_linked_list_t *)public;
    if (get_first(public, item) == SUCCESS)
    {
        remove_element(this, this->first);
        return SUCCESS;
    }

    return NOT_FOUND;
}

static status_t remove_last(linked_list_t *public, void **item)
{
    private_linked_list_t *this = (private_linked_list_t *)public;
    if (get_last(public, item) == SUCCESS)
    {
        remove_element(this, this->last);
        return SUCCESS;
    }

    return NOT_FOUND;
}

static int remove(linked_list_t *public, void *item, bool (*compare)(void*,void*))
{
    int removed = 0;

    if (!item && !compare)
        return 0;
    
    private_linked_list_t *this = (private_linked_list_t *)public;
    element_t *current = this->first;

    while (current)
    {
        if (compare && compare(current->value, item) ||
                (!compare && current->value == item))
        {
            removed++;
            current = remove_element(this, current);
        }
        else 
        {
            current = current->next;
        }

    }

    return removed;
}

static void remove_at(linked_list_t *list_public, enumerator_t *enum_public)
{
    private_enumerator_t *enum_private = (private_enumerator_t *)enum_public;
    private_linked_list_t *list_private = (private_linked_list_t *)list_public;

    element_t *current;
    
    if (enum_private->current)
    {
        current = enum_private->current;
        enum_private->current = current->next;
        enum_private->public.venumerate = enumerate_next;
        remove_element(list_private, current);
    }

    return ;
}

static void invoke_offset(linked_list_t *public, size_t offset)
{
    private_linked_list_t *this = (private_linked_list_t *)public;
    element_t *current = this->first;
    void (**method)(void *);

    while (current)
    {
        method = current->value + offset;
        (*method)(current->value);
        current = current->next;
    }
    return ;
}

static linked_list_t *clone_offset(linked_list_t *public, size_t offset)
{
    private_linked_list_t *this = (private_linked_list_t *)public;
    element_t *current = this->first;

    linked_list_t *clone = linked_list_create();

    while (current)
    {
        void* (**method)(void*) = current->value + offset;
        clone->insert_last(clone, (*method)(current->value));
		current = current->next;
    }

    return clone;
}

static bool find_first(linked_list_t *public, linked_list_match_t match, void **item, ...)
{
    private_linked_list_t *this = (private_linked_list_t *)public;

    element_t *current = this->first;
    va_list args;
    bool matched = false;

    if (!match && !item)
        return false;

    while (current)
    {
        if (match)
        {
            va_start(args, item);
            matched = match(current->value, args);
            va_end(args);
        }
        else 
        {
            matched = current->value == *item;
        }

        if (matched)
        {
            if (item != NULL)
                *item = current->value;

            return true;
        }

        current = current->next;
    }

    return false;
}

static void insert_before(linked_list_t *list_public, enumerator_t *enum_public, void *item)
{
    private_linked_list_t *list_private = (private_linked_list_t *)list_public;
    private_enumerator_t *enum_private = (private_enumerator_t *)enum_public;

    element_t *current, *elem;

    current = enum_private->current;
    if (!current)
    {
        insert_last(list_public, item);
    }
    elem = element_create(item);
    if (current->prev)
    {
        current->prev->next = elem;
        elem->prev = current->prev;
        current->prev = elem;
        elem->next = current;
    }
    else 
    {
        current->prev = elem;
        elem->next = current;
        list_private->first = elem;
    }
    list_private->count++;
    return ;
}

static void destroy(linked_list_t *this)
{
    void *value;

    while (remove_first(this, &value) == SUCCESS)
    {
        /* values are not destroyed so memory leaks are possible
		 * if list is not empty when deleting */
    }

    free(this);

    return ;
}

linked_list_t *linked_list_create()
{
    private_linked_list_t *this;
    this = calloc(1, sizeof(*this));

    this->public.get_count = get_count;
    this->public.create_enumerator = create_enumerator;
    this->public.reset_enumerator = reset_enumerator;
    this->public.get_first = get_first;
    this->public.get_last = get_last;
    this->public.find_first = find_first;
    this->public.insert_first = insert_first;
    this->public.insert_last = insert_last;
    this->public.insert_before = insert_before;
    this->public.remove_first = remove_first;
    this->public.remove_last = remove_last;
    this->public.remove = remove;
    this->public.remove_at = remove_at;
    this->public.invoke_offset = invoke_offset;
    this->public.clone_offset = clone_offset;

    this->public.invoke_function = invoke_function;
    this->public.destroy = destroy;
    
    return &this->public;
}