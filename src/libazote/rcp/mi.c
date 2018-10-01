#include <libazote/libazote.h>

uint32_t azRcpReadMI(AzState* state, uint32_t addr)
{
    switch (addr)
    {
    case MI_INIT_MODE_REG:
        // TODO: Implement me
        return 0;
    case MI_VERSION_REG:
        return 0x01010101;
    case MI_INTR_REG:
        return state->miRegisters[2];
    case MI_INTR_MASK_REG:
        return state->miRegisters[3];
    }
    return 0;
}

void azRcpWriteMI(AzState* state, uint32_t addr, uint32_t value)
{
    switch (addr)
    {
    case MI_INIT_MODE_REG:
        return;
    case MI_VERSION_REG:
        return;
    case MI_INTR_REG:
        return;
    case MI_INTR_MASK_REG:
        if (value & 0x0001) state->miRegisters[3] &= ~MI_INTR_SP;
        else if (value & 0x0002) state->miRegisters[3] |= MI_INTR_SP;
        if (value & 0x0004) state->miRegisters[3] &= ~MI_INTR_SI;
        else if (value & 0x0008) state->miRegisters[3] |= MI_INTR_SI;
        if (value & 0x0010) state->miRegisters[3] &= ~MI_INTR_AI;
        else if (value & 0x0020) state->miRegisters[3] |= MI_INTR_AI;
        if (value & 0x0040) state->miRegisters[3] &= ~MI_INTR_VI;
        else if (value & 0x0080) state->miRegisters[3] |= MI_INTR_VI;
        if (value & 0x0100) state->miRegisters[3] &= ~MI_INTR_PI;
        else if (value & 0x0200) state->miRegisters[3] |= MI_INTR_PI;
        if (value & 0x0400) state->miRegisters[3] &= ~MI_INTR_DP;
        else if (value & 0x0800) state->miRegisters[3] |= MI_INTR_DP;
        azRcpCheckInterrupts(state);
        return;
    }
}
