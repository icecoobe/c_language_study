
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// TODO: 
// 1.How free() works is more important than malloc(). 
// https://stackoverflow.com/questions/1119134/how-do-malloc-and-free-work
// malloc, free/new delete are both os dependent.
// in general, the heap mem was a linked list, each block is a chunk with
// meta data stored in previous location before the address malloc returned to user space app.
// so if there's some wrong operation on pointer, it maybe break the structure of mem management.

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
