#include <stdarg.h>
#include <stdio.h>
#include <sys/ucontext.h>

#include "collections/enumerator.h"
#include "collections/linked_list.h"
#include "utils/utils.h"

typedef struct job_t job_t;

static void print_id(job_t *this);
static job_t *job_clone(job_t *old);

static struct job_t
{
    int id;
    int status;
    void (*print_id)(job_t *this);
    job_t *(*clone)(job_t *this);
} jobs_arr[] = 
{
    {.id = 0, .status = 0, .print_id = print_id, .clone = job_clone},
    {.id = 1, .status = 0, .print_id = print_id, .clone = job_clone},
    {.id = 2, .status = 0, .print_id = print_id, .clone = job_clone},
    {.id = 3, .status = 0, .print_id = print_id, .clone = job_clone},
    {.id = 4, .status = 0, .print_id = print_id, .clone = job_clone},
    {.id = 5, .status = 0, .print_id = print_id, .clone = job_clone},
    {.id = 6, .status = 0, .print_id = print_id, .clone = job_clone},
    {.id = 7, .status = 0, .print_id = print_id, .clone = job_clone}
};

static void print_id(job_t *this)
{
    fprintf(stderr, "[%s%d] job id is %d, job status is %d\n", "invoke_offset", 1,
                                        this->id, this->status);
    return ;
}

static void list_invoke(job_t *entry, va_list args)
{
    char *group;
    int level;
    FILE *fp;

    VA_ARGS_VGET(args, fp, group, level);
    fprintf(fp, "[%s%d] job id : %d, job status : %d\n", 
                                group, level, entry->id,entry->status);
    return ;
}

static bool match(job_t *entry, va_list args)
{
    int id;
    VA_ARGS_VGET(args, id);
    return (id == entry->id);
}

static bool compare(job_t *curr, job_t *item)
{
    return curr->id < item->id;
}

static job_t *job_clone(job_t *old)
{
    return old;
}

int linked_list_test(void)
{
    job_t *entry;
    enumerator_t *e;
    linked_list_t *jobs, *jobs_clone;
    
    jobs =  linked_list_create();

    jobs->insert_last(jobs, &jobs_arr[1]);
    jobs->insert_last(jobs, &jobs_arr[2]);
    jobs->insert_last(jobs, &jobs_arr[4]);
    jobs->insert_first(jobs, &jobs_arr[0]);
    jobs->invoke_function(jobs, (linked_list_invoke_t)list_invoke, stderr, "invoke_function", 1);
    
    jobs->get_first(jobs, (void **)&entry);
    fprintf(stderr, "[%s%d] job id : %d, job status : %d\n", "get_first", 
                                            1, entry->id, entry->status);

    jobs->get_last(jobs, (void **)&entry);
    fprintf(stderr, "[%s%d] job id : %d, job status : %d\n", "get_last", 
                                            1, entry->id, entry->status);

    if (jobs->find_first(jobs, NULL, (void **)&jobs_arr[4]) == false)
        fprintf(stderr, "[%s%d] can not find job id : %d\n", "find_first", 
                                            1, jobs_arr[4].id);

    if (jobs->find_first(jobs, NULL, (void **)&entry) == true)
        fprintf(stderr, "[%s%d] find job id : %d, job status : %d\n", "find_first", 
                                            1, entry->id, entry->status);

    if (jobs->find_first(jobs, (linked_list_match_t)match, (void **)&entry, 4) == true)
        fprintf(stderr, "[%s%d] find job id : %d, job status : %d\n", "find_first", 
                                            1, entry->id, entry->status);

    fprintf(stderr, "jobs list size is %d\n", jobs->get_count(jobs));
    jobs->remove_first(jobs, (void **)&entry);
    jobs->remove_last(jobs, (void **)&entry);
    fprintf(stderr, "[%s%d] job id : %d, job status : %d\n", "remove_last", 
                                            1, entry->id, entry->status);
    fprintf(stderr, "[%s%d] jobs list size is %d\n", "remove_last", 
                                            1, jobs->get_count(jobs));
    jobs->invoke_function(jobs, (linked_list_invoke_t)list_invoke, stderr, "invoke_function", 1);

    fprintf(stderr, "instert all jobs from jobs_arr[]\n");
    jobs->insert_last(jobs, &jobs_arr[3]);
    jobs->insert_last(jobs, &jobs_arr[4]);
    jobs->insert_last(jobs, &jobs_arr[5]);
    jobs->insert_last(jobs, &jobs_arr[6]);
    jobs->insert_last(jobs, &jobs_arr[7]);
    jobs->insert_first(jobs, &jobs_arr[0]);
    jobs->invoke_function(jobs, (linked_list_invoke_t)list_invoke, stderr, "invoke_function", 1);

    e = jobs->create_enumerator(jobs);
    while (e->enumerate(e, &entry))
        fprintf(stderr, "[%s%d] job id : %d, job status : %d\n", 
                            "enumerate", 1, entry->id, entry->status);

    e->destroy(e);

    e = jobs->create_enumerator(jobs);
    jobs->remove_at(jobs, e);
    jobs->invoke_function(jobs, (linked_list_invoke_t)list_invoke, stderr, "invoke_function", 1);

    jobs->insert_before(jobs, e, &jobs_arr[0]);
    jobs->invoke_function(jobs, (linked_list_invoke_t)list_invoke, stderr, "invoke_function", 1);
    e->destroy(e);

    jobs->invoke_offset(jobs, offsetof(job_t, print_id));

    jobs_clone = jobs->clone_offset(jobs, offsetof(job_t, clone));
    jobs_clone->invoke_function(jobs_clone, (linked_list_invoke_t)list_invoke, 
                                                        stderr, "clone list invoke_function", 1);
    jobs_clone->destroy(jobs_clone);

    jobs->remove(jobs, &jobs_arr[4], (bool (*)(void *, void *))compare);
    jobs->invoke_function(jobs, (linked_list_invoke_t)list_invoke, stderr, "invoke_function", 1);

    jobs->destroy(jobs);
    return 0;
}