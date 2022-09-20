
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// TODO: aligned_alloc demo
// https://blog.csdn.net/jin739738709/article/details/122992753

int main(void)
{
    char *str;
 
    str = (char *) malloc(15);
    strcpy(str, "google");
    printf("String = %s,  Address = %p\n", str, str);

    str = (char *) realloc(str, 25);
    strcat(str, ".com");
    printf("String = %s,  Address = %p\n", str, str);

    free(str);

    return 0;
}
