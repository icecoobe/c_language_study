/*
gcc thread_test_1.c -o test -pthread
*/

#include <stdio.h>
#include <threads.h>
#include <stdlib.h>

static int thread_entry(void *arg);

int main(void)
{
    printf("C11 STD thread test.\n");

#if !defined (__STDC_NO_THREADS__)
#pragma "support std threads"

    thrd_t thread;
    int result;

    thrd_create(&thread, thread_entry, "hello c11 thread with id: %X.\n");
    
    thrd_t thrd = thrd_current();
    for (int i = 0; i < 5; i++)
    {
        thrd_sleep(&(struct timespec){.tv_sec=1}, NULL);
        printf("\r[%X] %04d", thrd, i);
    }

    if (thrd_join(thread, &result) == thrd_success)
    {
        printf("Thread returns %d at the end.\n", result);
    }

#else
#pragma message("dont support std threads")
#endif

    return 0;
}

int 
thread_entry(void *arg)
{
    thrd_t thrd = thrd_current();
    printf((const char*)arg, thrd);
    struct timespec t;
    t.tv_sec = 3;
    for (int i = 0; i < 5; i++)
    {
        thrd_sleep(&(struct timespec){.tv_sec=1}, NULL);
        printf("\r[%X] %04d", thrd, i);
    }

    return thrd_success;
}
