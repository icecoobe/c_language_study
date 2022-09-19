
#include <stdio.h>

/*
output:

0, -256
-2, 254
-128, 254

*/
int main(void)
{
    char a = -128;
    char b = 127;
    char result = (char)(a + a);
    printf("%d, %d\n", result, a + a);

    result = b + b;
    printf("%d, %d\n", result, b + b);

    result = b + 1;
    printf("%d, %d\n", result, b + b);

    return 0;
}
