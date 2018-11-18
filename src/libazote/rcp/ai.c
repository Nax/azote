#include <stdio.h>
#include <libazote/libazote.h>

static void dmaAudio(AzState* state)
{
    state->aiRegisters[1] = 0;
    azRcpRaiseInterrupt(state, RCP_INTR_AI);
}

uint32_t azRcpReadAI(AzState* state, uint32_t addr)
{
    //printf("AI Read:  0x%08x\n", addr);
    switch (addr)
    {
    case AI_DRAM_ADDR_REG:
        return 0;
    case AI_LEN_REG:
        return 0;
        return state->aiRegisters[1];
    case AI_CONTROL_REG:
        return 0;
    case AI_STATUS_REG:
        return state->aiRegisters[3];
    case AI_DACRATE_REG:
        return 0;
    case AI_BITRATE_REG:
        return 0;
    default:
        return 0;
    }
}

void azRcpWriteAI(AzState* state, uint32_t addr, uint32_t value)
{
    //printf("AI Write: 0x%08x (0x%08x)\n", addr, value);
    switch (addr)
    {
    case AI_DRAM_ADDR_REG:
        state->aiRegisters[0] = value;
        return;
    case AI_LEN_REG:
        return;
    case AI_CONTROL_REG:
        if (value & 0x01)
            dmaAudio(state);
        return;
    case AI_STATUS_REG:
        azRcpClearInterrupt(state, RCP_INTR_AI);
        return;
    case AI_DACRATE_REG:
        state->aiRegisters[4] = value;
        return;
    case AI_BITRATE_REG:
        state->aiRegisters[5] = value;
        return;
    default:
        return;
    }
}
