#include <stdio.h>
#include <libazote/libazote.h>

void azRcpClearInterrupt(AzState* state, uint8_t intr)
{
    state->miRegisters[2] &= ~(1 << intr);
    azRcpCheckInterrupts(state);
}

void azRcpRaiseInterrupt(AzState* state, uint8_t intr)
{
    state->miRegisters[2] |= (1 << intr);
    azRcpCheckInterrupts(state);
}

void azRcpCheckInterrupts(AzState* state)
{
    if (state->miRegisters[2] & state->miRegisters[3])
        state->cop0.registers[13] |= (1 << 10);
    else
        state->cop0.registers[13] &= ~(1 << 10);
}
