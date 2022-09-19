/*
gcc thread_test_1.c -o test -pthread
*/

#include <stdio.h>
#include <threads.h>
#include <stdlib.h>

static int test(void *arg);

int main(void)
{
    printf("C11 STD thread test.\n");

#if !defined (__STDC_NO_THREADS__)
#pragma "support std threads"

    thrd_t thread;
    int result;

    thrd_create(&thread, test, "hello c11 thread with id: %lu.\n");
    
    for (int i = 0; i < 1000; i++)
    {
        printf("\r%04d", i);
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

static int 
test(void *arg)
{
    thrd_t thrd = thrd_current();
    printf((const char*)arg, thrd);
    struct timespec t;
    t.tv_sec = 3;
    thrd_sleep(&(struct timespec){.tv_sec=5}, NULL);

    return thrd_success;
}
