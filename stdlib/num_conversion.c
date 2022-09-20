
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
output:

1.000000
Range error, got: 9223372036854775808.000000
0.000000
2000000000.000000

*/
int main(void)
{
    const char* strA = "1.0"; 
    printf("%f\n", atof(strA));

    const char* strB = "200000000000000000000000000000.0";
    char* end;
    double num = strtol(strB, &end, 10);
    if (errno == ERANGE)
    {
        printf("Range error, got: ");
        errno = 0;
    }
    printf("%f\n", num);

    strB = "200000000000000000000000000000.0";
    num = strtol(strB, &end, 50);
    if (errno == ERANGE)
    {
        printf("Range error, got: ");
        errno = 0;
    }
    printf("%f\n", num); // no error, but incorrect value: 0

    strB = "2000000000.0";
    num = strtol(strB, &end, 10);
    if (errno == ERANGE)
    {
        printf("Range error, got: ");
        errno = 0;
    }
    printf("%f\n", num);

    return 0;
}
