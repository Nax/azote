#include <libazote/libazote.h>

static void _dmaWrite(AzState* state)
{
    uint64_t size;
    uint32_t ramAddr;
    uint32_t romAddr;

    puts("DMA (Write)");
    size = (uint64_t)state->piRegisters[3] + 1;
    ramAddr = state->piRegisters[0];
    romAddr = state->piRegisters[1];
    printf("0x%08x -> 0x%08x (%llu bytes)\n", romAddr, ramAddr, size);
    //getchar();
    memcpy(state->rdram + ramAddr, state->cart + (romAddr & 0x0fffffff), size);
    azRcpRaiseInterrupt(state, RCP_INTR_PI);
}

uint32_t azRcpReadSP(AzState* state, uint32_t addr)
{
    switch (addr)
    {
    default:
        return 0;
    }
}

void azRcpWriteSP(AzState* state, uint32_t addr, uint32_t value)
{
    switch (addr)
    {
    default:
        return;
    }
}
