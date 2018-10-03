#include <libazote/libazote.h>

AZOTE_PROTO_COP(azOpMTC0)
{
    if (rd == 12)
    {
        //printf("STATUS: 0x%016llx    PC: 0x%016llx\n", state->cpu.registers[rt]);
        // TODO: Catch this
    }
    state->cop0.registers[rd] = state->cpu.registers[rt];
}

AZOTE_PROTO_COP(azOpMFC0)
{
    if (rt)
    {
        state->cpu.registers[rt] = state->cop0.registers[rd];
    }
}

AZOTE_PROTO_COPCO(azOpTLBR)
{
    uint8_t index = state->cop0.registers[COP0_REG_INDEX] & 0x1f;
    state->cop0.registers[COP0_REG_ENTRYLO0] = state->tlb.lo0[index];
    state->cop0.registers[COP0_REG_ENTRYLO1] = state->tlb.lo1[index];
    state->cop0.registers[COP0_REG_ENTRYHI] = state->tlb.hi[index];
    state->cop0.registers[COP0_REG_PAGE_MASK] = state->tlb.mask[index];
}

AZOTE_PROTO_COPCO(azOpTLBWI)
{
    uint8_t index = state->cop0.registers[COP0_REG_INDEX] & 0x1f;
    state->tlb.lo0[index] = state->cop0.registers[COP0_REG_ENTRYLO0];
    state->tlb.lo1[index] = state->cop0.registers[COP0_REG_ENTRYLO1];
    state->tlb.hi[index] = state->cop0.registers[COP0_REG_ENTRYHI];
    state->tlb.mask[index] = state->cop0.registers[COP0_REG_PAGE_MASK];

    printf("TLBWI:\n");
    printf("  Index:        0x%016llx\n", state->cop0.registers[COP0_REG_INDEX]);
    printf("  Mask:         0x%016llx\n", state->cop0.registers[COP0_REG_PAGE_MASK]);
    printf("  EntryHi:      0x%016llx\n", state->cop0.registers[COP0_REG_ENTRYHI]);
    printf("  EntryLo0:     0x%016llx\n", state->cop0.registers[COP0_REG_ENTRYLO0]);
    printf("  EntryLo1:     0x%016llx\n", state->cop0.registers[COP0_REG_ENTRYLO1]);
    getchar();
}

AZOTE_PROTO_COPCO(azOpTLBWR)
{
    printf("TLBWR:\n");
    printf("  Index:        0x%016llx\n", state->cop0.registers[COP0_REG_INDEX]);
    printf("  Mask:         0x%016llx\n", state->cop0.registers[COP0_REG_PAGE_MASK]);
    printf("  EntryHi:      0x%016llx\n", state->cop0.registers[COP0_REG_ENTRYHI]);
    printf("  EntryLo0:     0x%016llx\n", state->cop0.registers[COP0_REG_ENTRYLO0]);
    printf("  EntryLo1:     0x%016llx\n", state->cop0.registers[COP0_REG_ENTRYLO1]);
    getchar();
}

AZOTE_PROTO_COPCO(azOpTLBP)
{
    uint64_t entryHi = state->cop0.registers[COP0_REG_ENTRYHI];
    for (uint8_t index = 0; index < 0x1f; ++index)
    {
        uint64_t tlbHi = state->tlb.hi[index];
        if (((entryHi & 0xfffff000) == (tlbHi & 0xfffff000))
            && ((tlbHi & 0x800) || ((entryHi & 0xff) == (tlbHi & 0xff))))
        {
            state->cop0.registers[COP0_REG_INDEX] = index;
            return;
        }
    }
    state->cop0.registers[COP0_REG_INDEX] = state->cop0.registers[COP0_REG_INDEX] | (1 << 31);
}

AZOTE_PROTO_COPCO(azOpERET)
{
    if (state->cop0.registers[12] & (1 << 2))
    {
        state->cop0.registers[12] &= 0xfffffffffffffffb;
        state->cpu.pc = state->cop0.registers[30];
    }
    else
    {
        state->cop0.registers[12] &= 0xfffffffffffffffd;
        state->cpu.pc = state->cop0.registers[14];
    }
    state->cpu.pc2 = state->cpu.pc + 4;
    state->cpu.ll = 0;
}
