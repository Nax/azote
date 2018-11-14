#include <libazote/libazote.h>

uint32_t azRcpReadVI(AzState* state, uint32_t addr)
{
    printf("VI Read:  0x%08x\n", addr);
    switch (addr)
    {
    case VI_STATUS_REG:
        return state->viRegisters[0];
    case VI_ORIGIN_REG:
        return state->viRegisters[1];
    case VI_WIDTH_REG:
        return state->viRegisters[2];
    case VI_INTR_REG:
        return state->viRegisters[3];
    case VI_CURRENT_REG:
        return state->viRegisters[4];
    case VI_BURST_REG:
        return state->viRegisters[5];
    case VI_V_SYNC_REG:
        return state->viRegisters[6];
    case VI_H_SYNC_REG:
        return state->viRegisters[7];
    case VI_LEAP_REG:
        return state->viRegisters[8];
    case VI_H_START_REG:
        return state->viRegisters[9];
    case VI_V_START_REG:
        return state->viRegisters[10];
    case VI_V_BURST_REG:
        return state->viRegisters[11];
    case VI_X_SCALE_REG:
        return state->viRegisters[12];
    case VI_Y_SCALE_REG:
        return state->viRegisters[13];
    default:
        return 0;
    }
}

void azRcpWriteVI(AzState* state, uint32_t addr, uint32_t value)
{
    printf("VI Write: 0x%08x (0x%08x)\n", addr, value);
    switch (addr)
    {
    case VI_STATUS_REG:
        state->viRegisters[0] = value;
        return;
    case VI_ORIGIN_REG:
        state->viRegisters[1] = value;
        return;
    case VI_WIDTH_REG:
        state->viRegisters[2] = value;
        return;
    case VI_INTR_REG:
        state->viRegisters[3] = value;
        return;
    case VI_CURRENT_REG:
        azRcpClearInterrupt(state, RCP_INTR_VI);
        return;
    case VI_BURST_REG:
        state->viRegisters[5] = value;
        return;
    case VI_V_SYNC_REG:
        state->viRegisters[6] = value;
        return;
    case VI_H_SYNC_REG:
        state->viRegisters[7] = value;
        return;
    case VI_LEAP_REG:
        state->viRegisters[8] = value;
        return;
    case VI_H_START_REG:
        state->viRegisters[9] = value;
        return;
    case VI_V_START_REG:
        state->viRegisters[10] = value;
        return;
    case VI_V_BURST_REG:
        state->viRegisters[11] = value;
        return;
    case VI_X_SCALE_REG:
        state->viRegisters[12] = value;
        return;
    case VI_Y_SCALE_REG:
        state->viRegisters[13] = value;
        return;
    default:
        return;
    }
}
