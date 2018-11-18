#include <stdio.h>
#include <libazote/libazote.h>

void azRcpClearInterrupt(AzState* state, uint8_t intr)
{
    state->miRegisters[2] &= ~(1 << intr);
    azRcpCheckInterrupts(state);
    if (intr == RCP_INTR_DP)
    {
        puts("\n\nFRAME SYNC END\n\n");
        pthread_mutex_lock(&state->viMutex);
        state->viSync = 1;
        pthread_mutex_unlock(&state->viMutex);
        pthread_cond_signal(&state->viCond);
    }
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
