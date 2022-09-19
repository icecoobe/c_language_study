
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define NAMEOF(x) (#x)

volatile sig_atomic_t sig = 0;

static void signal_handler(int signal);
static void regist_expected_signals(void);
static void exit_handler(void);

int main(void)
{
    atexit(exit_handler);
    regist_expected_signals();

    raise(SIGILL);
    raise(SIGINT);

    // while(1);

    int x = 10;
    int y = 0;
    printf("%s = %d\n", NAMEOF(x / y), x/y);
    printf("%s = %d\n", NAMEOF(x), x);
    
    return 0;
}

static void 
signal_handler(int signal)
{
    printf("[ERROR] Signal %d catched!\n", signal);
    sig = signal;
    exit(sig);
}

static void 
regist_expected_signals(void)
{
    // NOTE: there are 3 ways to handle signal
    // signal(SIGNAME, SIG_IGN); // ignore the signal
    // signal(SIGNAME, SIG_DFL); // use default handler
    // signal(SIGNAME, signal_handler); // specified handler

    //signal(SIGABRT, signal_handler);
    signal(SIGFPE, signal_handler);
    signal(SIGILL, SIG_DFL); // invalid hw instruction
    signal(SIGINT, signal_handler); // e.g. Ctrl + C
    //signal(SIGSEGV, signal_handler); // invalid memory reference
    signal(SIGTERM, SIG_IGN); // termination signal
}

static void 
exit_handler(void)
{
    // int atexit(void (*func)(void))
    // do something before exit
    // make sure here cannot bring other errors, 
    // which will leads to termination
    printf("[WARNING] Application is about to exit ...\n");

    // TODO: add clean up here
}
