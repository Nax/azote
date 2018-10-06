#include <libazote/libazote.h>

uint32_t azRcpReadVI(AzState* state, uint32_t addr)
{
    printf("VI Read:  0x%08x\n", addr);
    switch (addr)
    {
    default:
        return 0;
    }
}

void azRcpWriteVI(AzState* state, uint32_t addr, uint32_t value)
{
    printf("VI Write: 0x%08x (0x%08x)\n", addr, value);
    switch (addr)
    {
    case VI_CURRENT_REG:
        azRcpClearInterrupt(state, RCP_INTR_VI);
        return;
    }
}
