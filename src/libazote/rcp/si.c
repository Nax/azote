#include <libazote/libazote.h>

uint32_t azRcpReadSI(AzState* state, uint32_t addr)
{
    printf("SI Read:  0x%08x\n", addr);
    switch (addr)
    {
    default:
        return 0;
    }
}

void azRcpWriteSI(AzState* state, uint32_t addr, uint32_t value)
{
    printf("SI Write: 0x%08x (0x%08x)\n", addr, value);
    switch (addr)
    {
    default:
        return;
    }
}
