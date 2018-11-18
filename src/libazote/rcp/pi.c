#include <stdio.h>
#include <libazote/libazote.h>

static void _dmaRead(AzState* state)
{
    uint64_t size;
    uint32_t ramAddr;
    uint32_t romAddr;

    //puts("DMA (Read)");
    size = (uint64_t)state->piRegisters[3] + 1;
    ramAddr = state->piRegisters[0];
    romAddr = state->piRegisters[1];
    //printf("0x%08x -> 0x%08x (%llu bytes)\n", romAddr, ramAddr, size);
    //getchar();
    memcpy(state->rdram + ramAddr, state->cart + (romAddr & 0x0fffffff), size);
    azRcpRaiseInterrupt(state, RCP_INTR_PI);
}

static void _dmaWrite(AzState* state)
{
    uint64_t size;
    uint32_t ramAddr;
    uint32_t romAddr;

    ///puts("DMA (Write)");
    size = (uint64_t)state->piRegisters[3] + 1;
    ramAddr = state->piRegisters[0];
    romAddr = state->piRegisters[1];
    //printf("0x%08x -> 0x%08x (%llu bytes)\n", romAddr, ramAddr, size);
    //getchar();
    memcpy(state->rdram + ramAddr, state->cart + (romAddr & 0x0fffffff), size);
    azRcpRaiseInterrupt(state, RCP_INTR_PI);
}

uint32_t azRcpReadPI(AzState* state, uint32_t addr)
{
    //printf("PI Read:  0x%08x\n", addr);
    switch (addr)
    {
    case PI_DRAM_ADDR_REG:
        return state->piRegisters[0];
    case PI_CART_ADDR_REG:
        return state->piRegisters[1];
    case PI_RD_LEN_REG:
        return state->piRegisters[2];
    case PI_WR_LEN_REG:
        return state->piRegisters[3];
    case PI_STATUS_REG:
        return 0;
    case PI_BSD_DOM1_LAT_REG:
        return state->piRegisters[5];
    case PI_BSD_DOM1_PWD_REG:
        return state->piRegisters[6];
    case PI_BSD_DOM1_PGS_REG:
        return state->piRegisters[7];
    case PI_BSD_DOM1_RLS_REG:
        return state->piRegisters[8];
    case PI_BSD_DOM2_LAT_REG:
        return state->piRegisters[9];
    case PI_BSD_DOM2_PWD_REG:
        return state->piRegisters[10];
    case PI_BSD_DOM2_PGS_REG:
        return state->piRegisters[11];
    case PI_BSD_DOM2_RLS_REG:
        return state->piRegisters[12];
    default:
        return 0;
    }
}

void azRcpWritePI(AzState* state, uint32_t addr, uint32_t value)
{
    //printf("PI Write: 0x%08x (0x%08x)\n", addr, value);
    switch (addr)
    {
    case PI_DRAM_ADDR_REG:
        state->piRegisters[0] = value;
        return;
    case PI_CART_ADDR_REG:
        state->piRegisters[1] = value;
        return;
    case PI_RD_LEN_REG:
        state->piRegisters[2] = value;
        // TODO: Implement this
        _dmaRead(state);
        return;
    case PI_WR_LEN_REG:
        state->piRegisters[3] = value;
        _dmaWrite(state);
        return;
    case PI_STATUS_REG:
        if (value & 0x02)
            azRcpClearInterrupt(state, RCP_INTR_PI);
        return;
    case PI_BSD_DOM1_LAT_REG:
        state->piRegisters[5] = value;
        return;
    case PI_BSD_DOM1_PWD_REG:
        state->piRegisters[6] = value;
        return;
    case PI_BSD_DOM1_PGS_REG:
        state->piRegisters[7] = value;
        return;
    case PI_BSD_DOM1_RLS_REG:
        state->piRegisters[8] = value;
        return;
    case PI_BSD_DOM2_LAT_REG:
        state->piRegisters[9] = value;
        return;
    case PI_BSD_DOM2_PWD_REG:
        state->piRegisters[10] = value;
        return;
    case PI_BSD_DOM2_PGS_REG:
        state->piRegisters[11] = value;
        return;
    case PI_BSD_DOM2_RLS_REG:
        state->piRegisters[12] = value;
        return;
    default:
        return;
    }
}
