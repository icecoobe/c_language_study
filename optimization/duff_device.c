/*
Duff's Device

Modern c compilers are smart enough to optimize (unrolling the loop).
Here I just practise the mechanism. 
*/

#include <stdio.h>
#include <time.h>

static void send(int * to, int * from, int count) 
{
	for (int i = 0; i < count; i++) 
    {
		*to++ = *from++;
	}
}

// NOTE: for production, use memcpy is the best choice
static void send_duff(int * to, int * from, int count) 
{
    int n = (count + 7 ) / 8;

    switch (count % 8 ) {
    case 0 :    do { * to ++ = * from ++ ;
    case 7 :          * to ++ = * from ++ ;
    case 6 :          * to ++ = * from ++ ;
    case 5 :          * to ++ = * from ++ ;
    case 4 :          * to ++ = * from ++ ;
    case 3 :          * to ++ = * from ++ ;
    case 2 :          * to ++ = * from ++ ;
    case 1 :          * to ++ = * from ++ ;
           } while (--n > 0);
    }  
}

/*
Output:

Loop copy:	    0.000041 seconds
duff's device:	0.000010 seconds

*/
int main(void)
{
    clock_t clkBegin = clock();

    int num1[10024] = {0x1234};
    int num2[10024];

    send(num2, num1, 10024);

    // the real cpu time, exclude the sleep period
    // so the duration is much less than 3s
    clock_t clkEnd = clock();
    printf("Loop copy:\t%lf seconds\n", 
        (clkEnd - clkBegin) / (float)CLOCKS_PER_SEC);

    clkBegin = clock();

    send_duff(num2, num1, 10024);
    
    // the real cpu time, exclude the sleep period
    // so the duration is much less than 3s
    clkEnd = clock();
    printf("duff's device:\t%lf seconds\n", 
        (clkEnd - clkBegin) / (float)CLOCKS_PER_SEC);

    return 0;
}
