#include <libazote/libazote.h>

uint64_t azGetTimeNano(void)
{
    LARGE_INTEGER counter;
    LARGE_INTEGER frequency;

    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&frequency);
    return counter.QuadPart * 1000000000 / frequency.QuadPart;
}
