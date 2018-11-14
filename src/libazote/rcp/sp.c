#include <stdio.h>
#include <string.h>
#include <libazote/libazote.h>

static void _dmaReadDRAM(AzState* state)
{
    uint64_t size;
    uint32_t ramAddr;
    uint32_t spAddr;

    puts("SP DMA (Read)");
    size = (uint64_t)state->spRegisters[2] + 1;
    spAddr = state->spRegisters[0] & 0x1fff;
    ramAddr = state->spRegisters[1] & 0xffffff;
    printf("0x%08x <- 0x%08x (%llu bytes)\n", spAddr, ramAddr, size);
    if (spAddr & 0x1000)
        memcpy(state->spImem + (spAddr & 0xfff), state->rdram + ramAddr, size);
    else
        memcpy(state->spDmem + (spAddr & 0xfff), state->rdram + ramAddr, size);
}

static void _dmaWriteDRAM(AzState* state)
{
    uint64_t size;
    uint32_t ramAddr;
    uint32_t spAddr;

    puts("SP DMA (Write)");
    size = (uint64_t)state->spRegisters[2] + 1;
    ramAddr = state->piRegisters[0];
    spAddr = state->piRegisters[1];
    printf("0x%08x <- 0x%08x (%llu bytes)\n", spAddr, ramAddr, size);
    getchar();
    //memcpy(state->rdram + ramAddr, state->cart + (romAddr & 0x0fffffff), size);
    //azRcpRaiseInterrupt(state, RCP_INTR_PI);
}

uint32_t azRcpReadSP(AzState* state, uint32_t addr)
{
    printf("SP Read:  0x%08x\n", addr);
    switch (addr)
    {
    case SP_MEM_ADDR_REG:
        return state->spRegisters[0];
    case SP_DRAM_ADDR_REG:
        return state->spRegisters[1];
    case SP_RD_LEN_REG:
        return state->spRegisters[2];
    case SP_WR_LEN_REG:
        return state->spRegisters[3];
    case SP_STATUS_REG:
        return state->spRegisters[4];
    case SP_PC_REG:
        return state->rsp.pc & 0xfff;
    default:
        return 0;
    }
}

void azRcpWriteSP(AzState* state, uint32_t addr, uint32_t value)
{
    printf("SP Write: 0x%08x (0x%08x)\n", addr, value);
    switch (addr)
    {
    case SP_MEM_ADDR_REG:
        state->spRegisters[0] = value;
        return;
    case SP_DRAM_ADDR_REG:
        state->spRegisters[1] = value;
        return;
    case SP_RD_LEN_REG:
        state->spRegisters[2] = value;
        _dmaReadDRAM(state);
        return;
    case SP_WR_LEN_REG:
        state->spRegisters[3] = value;
        _dmaWriteDRAM(state);
        return;
    case SP_STATUS_REG:
        if (value & 0x00000001) state->spRegisters[4] &= ~(0x0001);             // clear halt
        else if (value & 0x00000002) state->spRegisters[4] |= 0x0001;           // set halt
        if (value & 0x00000004) state->spRegisters[4] &= ~(0x0002);             // clear broke
        if (value & 0x00000008) azRcpClearInterrupt(state, RCP_INTR_SP);        // clear intr
        else if (value & 0x00000010) azRcpRaiseInterrupt(state, RCP_INTR_SP);   // set intr
        if (value & 0x00000020) state->spRegisters[4] &= ~(0x0020);             // clear sstep
        else if (value & 0x00000040) state->spRegisters[4] |= 0x0020;           // set sstep
        if (value & 0x00000080) state->spRegisters[4] &= ~(0x0040);             // clear intr on break
        else if (value & 0x00000100) state->spRegisters[4] |= 0x0040;           // set intr on break
        if (value & 0x00000200) state->spRegisters[4] &= ~(0x0080);             // clear signal 0
        else if (value & 0x00000400) state->spRegisters[4] |= 0x0080;           // set signal 0
        if (value & 0x00000800) state->spRegisters[4] &= ~(0x0100);             // clear signal 1
        else if (value & 0x00001000) state->spRegisters[4] |= 0x0100;           // set signal 1
        if (value & 0x00002000) state->spRegisters[4] &= ~(0x0200);             // clear signal 2
        else if (value & 0x00004000) state->spRegisters[4] |= 0x0200;           // set signal 2
        if (value & 0x00008000) state->spRegisters[4] &= ~(0x0400);             // clear signal 3
        else if (value & 0x00010000) state->spRegisters[4] |= 0x0400;           // set signal 3
        if (value & 0x00020000) state->spRegisters[4] &= ~(0x0800);             // clear signal 4
        else if (value & 0x00040000) state->spRegisters[4] |= 0x0800;           // set signal 4
        if (value & 0x00080000) state->spRegisters[4] &= ~(0x1000);             // clear signal 5
        else if (value & 0x00100000) state->spRegisters[4] |= 0x1000;           // set signal 5
        if (value & 0x002000000) state->spRegisters[4] &= ~(0x2000);            // clear signal 6
        else if (value & 0x00400000) state->spRegisters[4] |= 0x2000;           // set signal 6
        if (value & 0x00800000) state->spRegisters[4] &= ~(0x4000);             // clear signal 7
        else if (value & 0x01000000) state->spRegisters[4] |= 0x4000;           // set signal 7
        printf("RSP: %s\n", (state->spRegisters[4] & 0x01) ? "HALT" : "RUN");
        if (!(state->spRegisters[4] & 0x01))
        {
            getchar();
            azRunRSP(state);
        }
        return;
    case SP_PC_REG:
        state->rsp.pc = value & 0xfff;
        state->rsp.pc2 = state->rsp.pc + 4;
        return;
    default:
        return;
    }
}
