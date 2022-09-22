#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Platform definition */
#if defined(_WIN32) || defined(_WIN64)
#   define aligned_alloc(alignment, size) _aligned_malloc(size, alignment)
#   define aligned_free  _aligned_free
#endif

static void *my_1st_aligned_alloc(size_t alignment, size_t size);
static void my_1st_aligned_free(void *mem);
static void *my_aligned_alloc(size_t alignment, size_t size);
static void my_aligned_free(void *mem);
static void *stl_aligned_alloc(size_t alignment, size_t size);
static void stl_aligned_free(void * aligned_ptr);
void test(size_t alignment, size_t size);

int main(void)
{
    printf("%llu, %llu\n", sizeof(size_t), sizeof(void*));
    void *ptemp = malloc(100);
    printf("ptemp is %p, check default alignment with 16 bytes: %llu\n",
           ptemp, (size_t)ptemp % 16);
    free(ptemp);

    size_t alignment = 32;
    size_t size = alignment * 4;
    void *p = NULL;

    p = aligned_alloc(alignment, size);
    printf("standard c lib allocated pointer is %p, "
           "check alignment: %d\n",
           p, (size_t)p % alignment == 0);
    aligned_free(p);

    alignment = 1024;
    size = alignment * 4;
    test(alignment, size);

    p = my_1st_aligned_alloc(alignment, size);
    printf("my 1st lib allocated pointer is %p, "
           "check alignment: %d\n",
           p, (size_t)p % alignment == 0);
    my_1st_aligned_free(p);

    p = my_aligned_alloc(alignment, size);
    printf("my lib allocated pointer is %p, "
           "check alignment: %d\n",
           p, (size_t)p % alignment == 0);
    my_aligned_free(p);

    p = stl_aligned_alloc(alignment, size);
    printf("STL lib allocated pointer is %p, "
           "check alignment: %d\n",
           p, (size_t)p % alignment == 0);
    stl_aligned_free(p);

    return 0;
}

void *
my_1st_aligned_alloc(size_t alignment, size_t size)
{
    static_assert(sizeof(void*) % 8 == 0, "System is not 8-bytes aligned!");
    assert((size % alignment == 0) && "Size must be multiple of alignment.");

    if (size % alignment != 0)
    {
        fprintf(stderr, "Size must be multiple of alignment.\n");
        return NULL;
    }

    // aligned addr - (raw - raw & ~(alignment - 1)) - sizeof(void*)
    const size_t real_size = size + alignment + sizeof(void*);
    void *praw = malloc(real_size);
    printf("%llX, %llX\n", ((size_t)praw) & ~(alignment - 1), alignment);
    printf("%llX\n", (((size_t)praw) & ~(alignment - 1)) + alignment);
    void *paligned_mem = (void *)((((size_t)praw) & ~(alignment - 1)) + alignment);
    void *pstore = (void*)((size_t)paligned_mem - sizeof(void*));
    //size_t offset = aligned addr - (praw - praw & ~(alignment - 1)) - sizeof(void*);
    *(size_t *)pstore = (size_t)praw;

    printf("real size=%llu, offset=%llu\n", real_size, (size_t)paligned_mem - (size_t)praw);
    assert((size_t)pstore % sizeof(void*) == 0);
    assert((size_t)paligned_mem % alignment == 0 && "Aligned memory is not aligned with requirement.");

    printf("pstore=%p, praw=%p\n", pstore, praw);
    return paligned_mem;
}

void
my_1st_aligned_free(void *mem)
{
    assert((size_t)mem % sizeof(void*) == 0);

    // calc the location storing raw mem address
    void *pstore = (void*)((size_t)mem - sizeof(void*));
    void *praw = (void*)*(size_t *)pstore;

    assert((size_t)pstore % sizeof(void*) == 0);

    printf("pstore=%p, praw=%p\n", pstore, praw);

    // free space with raw mem address
    free(praw);
}

void *
my_aligned_alloc(size_t alignment, size_t size)
{
    static_assert(sizeof(void*) % 8 == 0, "System is not 8-bytes aligned!");
    assert((size % alignment == 0) && "Size must be multiple of alignment.");

    if (size % alignment != 0)
    {
        fprintf(stderr, "Size must be multiple of alignment.\n");
        return NULL;
    }

    // aligned addr - (raw - raw & ~(alignment - 1)) - sizeof(void*)
    const size_t real_size = size + alignment + sizeof(void*);
    void *praw = malloc(real_size);

    printf("%llX\n", (((size_t)praw + alignment) & ~(alignment - 1)));

    void *paligned_mem = (void *)((((size_t)praw + alignment) & ~(alignment - 1)));
    void *pstore = (void*)((size_t)paligned_mem - sizeof(void*));
    //size_t offset = aligned addr - (praw - praw & ~(alignment - 1)) - sizeof(void*);
    *(size_t *)pstore = (size_t)praw;

    printf("real size=%llu, offset=%llu\n", real_size, (size_t)paligned_mem - (size_t)praw);
    assert((size_t)pstore % sizeof(void*) == 0);
    assert((size_t)paligned_mem % alignment == 0 && "Aligned memory is not aligned with requirement.");

    return paligned_mem;
}

void
my_aligned_free(void *mem)
{
    assert((size_t)mem % sizeof(void*) == 0);

    // calc the location storing raw mem address
    void *pstore = (void*)((size_t)mem - sizeof(void*));
    void *praw = (void*)*(size_t *)pstore;

    assert((size_t)pstore % sizeof(void*) == 0);

    // free space with raw mem address
    free(praw);
}

void *
stl_aligned_alloc(size_t alignment, size_t size)
{
    // 分配足够的内存, 这里的算法很经典, 早期的STL中使用的就是这个算法

    // 首先是维护FreeBlock指针占用的内存大小
    const int pointerSize = sizeof(void*);

    // alignment - 1 + pointerSize这个是FreeBlock内存对齐需要的内存大小
    // 前面的例子sizeof(T) = 20, __alignof(T) = 16,
    // g_MaxNumberOfObjectsInPool = 1000
    // 那么调用本函数就是alignedMalloc(1000 * 20, 16)
    // 那么alignment - 1 + pointSize = 19
    const int requestedSize = size + alignment - 1 + pointerSize;

    // 分配的实际大小就是20000 + 19 = 20019
    void* raw = malloc(requestedSize);

    // 这里实Pool真正为对象实例分配的内存地址
    uintptr_t start = (uintptr_t)raw + pointerSize;
    // 向上舍入操作
    // 解释一下, __ALIGN - 1指明的是实际内存对齐的粒度
    // 例如__ALIGN = 8时, 我们只需要7就可以实际表示8个数(0~7)
    // 那么~(__ALIGN - 1)就是进行舍入的粒度
    // 我们将(bytes) + __ALIGN-1)就是先进行进位, 然后截断
    // 这就保证了我是向上舍入的
    // 例如byte = 100, __ALIGN = 8的情况
    // ~(__ALIGN - 1) = (1 000)B
    // ((bytes) + __ALIGN-1) = (1 101 011)B
    // (((bytes) + __ALIGN-1) & ~(__ALIGN - 1)) = (1 101 000 )B = (104)D
    // 104 / 8 = 13, 这就实现了向上舍入
    // 对于byte刚好满足内存对齐的情况下, 结果保持byte大小不变
    // 记得《Hacker's Delight》上面有相关的计算
    // 这个表达式与下面给出的等价
    // ((((bytes) + _ALIGN - 1) * _ALIGN) / _ALIGN)
    // 但是SGI STL使用的方法效率非常高
    void* aligned = (void*)((start + alignment - 1) & ~(alignment - 1));

    // 这里维护一个指向malloc()真正分配的内存
    *(void**)((uintptr_t)aligned - pointerSize) = raw;

    printf("pstore=%p, praw=%p\n", *(void**)((uintptr_t)aligned - pointerSize), raw);
    printf("real size=%llu, offset=%llu\n", requestedSize, (size_t)aligned - (size_t)start);

    // 返回实例对象真正的地址
    return aligned;
}

void
stl_aligned_free(void * aligned_ptr)
{
    if (aligned_ptr)
    {
        free(((void**)aligned_ptr)[-1]);
    }
}

_Bool
is_aligned(void* data, int alignment)
{
    // 又是一个经典算法, 参见<Hacker's Delight>
    return ((uintptr_t)data & (alignment - 1)) == 0;
}

void
test(size_t alignment, size_t size)
{
    assert((size % alignment == 0) && "Size must be multiple of alignment.");
    static_assert(sizeof(void*) % 8 == 0, "System is not 8-bytes aligned!");
    //static_assert(sizeof(void*) % 64 == 0, "test static assert");

    if (size % alignment != 0)
    {
        fprintf(stderr, "Size must be multiple of alignment.\n");
        return NULL;
    }

    // aligned addr - (raw - raw & ~(alignment - 1)) - sizeof(void*)
    size_t real_size = size + alignment + sizeof(void*);
    void *praw = 0x1E30E3E8F0;//malloc(real_size);
    printf("%llX, %llX\n", ((size_t)praw) & ~(alignment - 1), alignment);
    printf("%llX\n", (((size_t)praw) & ~(alignment - 1)) + alignment);
    void *paligned_mem = (void *)((((size_t)praw) & ~(alignment - 1)) + alignment);
    void *pstore = (void*)((size_t)paligned_mem - sizeof(void*));
    //size_t offset = aligned addr - (praw - praw & ~(alignment - 1)) - sizeof(void*);
    //*(size_t *)pstore = (size_t)praw;

    assert((size_t)pstore % sizeof(void*) == 0);
    assert((size_t)paligned_mem % alignment == 0 && "Aligned memory is not aligned with requirement.");
}
