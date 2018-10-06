#include <libazote/libazote.h>

uint32_t azRcpReadAI(AzState* state, uint32_t addr)
{
    printf("AI Read:  0x%08x\n", addr);
    switch (addr)
    {
    default:
        return 0;
    }
}

void azRcpWriteAI(AzState* state, uint32_t addr, uint32_t value)
{
    printf("AI Write: 0x%08x (0x%08x)\n", addr, value);
    switch (addr)
    {
    default:
        return;
    }
}
