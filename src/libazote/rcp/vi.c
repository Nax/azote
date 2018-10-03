#include <libazote/libazote.h>

uint32_t azRcpReadVI(AzState* state, uint32_t addr)
{
    switch (addr)
    {
    default:
        return 0;
    }
}

void azRcpWriteVI(AzState* state, uint32_t addr, uint32_t value)
{
    switch (addr)
    {
    case VI_CURRENT_REG:
        azRcpClearInterrupt(state, RCP_INTR_VI);
        return;
    }
}
