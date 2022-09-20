
#include <stdio.h>
#include <time.h>

#include <unistd.h>

/*
There are two kinds of time in C:
1.Calendar time: time_t, timespect_t, struct tm
2.CPU time: clock_t
*/
int main(void)
{
    time_t currTime = time(NULL);
    if (currTime == (time_t)-1)
    {
        printf("failed to get current time!\n");
        return -1;
    }

    clock_t clkBegin = clock();

    printf("The current timestamp is: %ld(s)\n", currTime);
    struct tm *pt = localtime(&currTime);
    char *strTime = asctime(pt);
    printf("local time is %s\n", asctime(pt));
    printf("local time is %s\n", ctime(&currTime));

    
    pt = gmtime(&currTime);
    printf("UTC/GMT time is %s\n", asctime(pt));
    printf("UTC/GMT time is %s\n", ctime(&currTime));

    sleep(3);

    time_t now = time(NULL);
    printf("dif of time is :%lf seconds\n", difftime(now, currTime));

    // here we can find the *strTime was changed
    // which means the c lib use one global buffer to store time in string
    pt = localtime(&now);
    printf("local time is %s\n", asctime(pt));
    printf("local time is %s\n", strTime);

    char timeArray[100];
    struct tm t;
    strftime((char *)timeArray, 
        100, "%A %c %Y-%m-%d %H:%M:%S", &t);
    printf("%s\n", timeArray);
    strftime((char *)timeArray, 
        100, "%A %c %Y-%m-%d %H:%M:%S", pt);
    printf("%s\n", timeArray);

    // the real cpu time, exclude the sleep period
    // so the duration is much less than 3s
    clock_t clkEnd = clock();
    printf("%lu, %lu, %lf\n", 
        clkBegin, 
        clkEnd, 
        (clkEnd - clkBegin) / (float)CLOCKS_PER_SEC);

    return 0;
}
