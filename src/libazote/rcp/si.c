#include <stdio.h>
#include <libazote/libazote.h>

static void _dmaRead(AzState* state)
{
    //puts("SI DMA Read");
    azRcpRaiseInterrupt(state, RCP_INTR_SI);
    //getchar();
}

static void _dmaWrite(AzState* state)
{
    //puts("SI DMA Write");
    azRcpRaiseInterrupt(state, RCP_INTR_SI);
    //getchar();
}

uint32_t azRcpReadSI(AzState* state, uint32_t addr)
{
    //printf("SI Read:  0x%08x\n", addr);
    switch (addr)
    {
    case SI_DRAM_ADDR_REG:
        return state->siRegisters[0];
    case SI_PIF_ADDR_RD64B_REG:
        return state->siRegisters[1];
    case SI_PIF_ADDR_WR64B_REG:
        return state->siRegisters[2];
    case SI_STATUS_REG:
        return 0;
    default:
        return 0;
    }
}

void azRcpWriteSI(AzState* state, uint32_t addr, uint32_t value)
{
    //printf("SI Write: 0x%08x (0x%08x)\n", addr, value);
    switch (addr)
    {
    case SI_DRAM_ADDR_REG:
        state->siRegisters[0] = value;
        return;
    case SI_PIF_ADDR_RD64B_REG:
        _dmaRead(state);
        return;
    case SI_PIF_ADDR_WR64B_REG:
        _dmaWrite(state);
        return;
    case SI_STATUS_REG:
        azRcpClearInterrupt(state, RCP_INTR_SI);
        return;
    default:
        return;
    }
}
