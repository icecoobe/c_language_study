
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define TYPE_APPLE 1
#define TYPE_PEAR 2

#define NAMEOF(x) (#x)

#define MAJOR 0x01
#define MINOR 0x02
#define REV   0x03

#define VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define VER VERSION(MAJOR, MINOR, REV)

struct fruit_s {
    int _type;
    char* name;
};

#define DECLARE(x) \
struct fruit_s x = { \
    TYPE_##x,   \
    #x, \
};

DECLARE(APPLE)
DECLARE(PEAR)

/*
output:

010203
x = 10
APPLE._type = 1, APPLE.name = APPLE
2, PEAR

*/
int main(void)
{
    int x = 10;

    printf("%06X\n", VER);
    printf("%s = %d\n", NAMEOF(x), x);
    printf("%s = %d, %s = %s\n", NAMEOF(APPLE._type), APPLE._type, NAMEOF(APPLE.name), APPLE.name);
    printf("%d, %s\n", PEAR._type, PEAR.name);

    return 0;
}
