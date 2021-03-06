#include <stdio.h>
#include <libazote/libazote.h>

uint32_t azRcpReadMI(AzState* state, uint32_t addr)
{
    //printf("MI Read:  0x%08x\n", addr);
    switch (addr)
    {
    case MI_INIT_MODE_REG:
        return state->miRegisters[0];
    case MI_VERSION_REG:
        return 0x02020102;
    case MI_INTR_REG:
        return state->miRegisters[2];
    case MI_INTR_MASK_REG:
        return state->miRegisters[3];
    }
    return 0;
}

void azRcpWriteMI(AzState* state, uint32_t addr, uint32_t value)
{
    uint32_t interrupts;

    //printf("MI Write: 0x%08x (0x%08x)\n", addr, value);
    switch (addr)
    {
    case MI_INIT_MODE_REG:
        state->miRegisters[0] = (state->miRegisters[0] & ~(0x7f)) | (value & 0x7f);
        if (value & 0x0080) state->miRegisters[0] &= ~(0x0080);
        else if (value & 0x0100) state->miRegisters[0] |= 0x0080;
        if (value & 0x0200) state->miRegisters[0] &= ~(0x0100);
        else if (value & 0x0400) state->miRegisters[0] |= 0x0100;
        if (value & 0x0800) azRcpClearInterrupt(state, RCP_INTR_DP);
        if (value & 0x1000) state->miRegisters[0] &= ~(0x0200);
        else if (value & 0x2000) state->miRegisters[0] |= 0x0200;
        return;
    case MI_VERSION_REG:
        return;
    case MI_INTR_REG:
        return;
    case MI_INTR_MASK_REG:
        interrupts = state->miRegisters[3];
        if (value & 0x0001) interrupts &= ~MI_INTR_SP;
        else if (value & 0x0002) interrupts |= MI_INTR_SP;
        if (value & 0x0004) interrupts &= ~MI_INTR_SI;
        else if (value & 0x0008) interrupts |= MI_INTR_SI;
        if (value & 0x0010) interrupts &= ~MI_INTR_AI;
        else if (value & 0x0020) interrupts |= MI_INTR_AI;
        if (value & 0x0040) interrupts &= ~MI_INTR_VI;
        else if (value & 0x0080) interrupts |= MI_INTR_VI;
        if (value & 0x0100) interrupts &= ~MI_INTR_PI;
        else if (value & 0x0200) interrupts |= MI_INTR_PI;
        if (value & 0x0400) interrupts &= ~MI_INTR_DP;
        else if (value & 0x0800) interrupts |= MI_INTR_DP;
        state->miRegisters[3] = interrupts;
        azRcpCheckInterrupts(state);
        return;
    }
}
