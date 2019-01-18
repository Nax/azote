#include <libazote/libazote.h>

uint64_t azGetTimeNano(void)
{
    return clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
}
