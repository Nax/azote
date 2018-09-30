#include <libazote/libazote.h>

AzState* azInit()
{
    AzState* state;

    state = malloc(sizeof(*state));
    memset(state, 0, sizeof(*state));
    state->rdram = malloc(AZOTE_MEMORY_SIZE);
    memset(state->rdram, 0, AZOTE_MEMORY_SIZE);
    state->spDmem = malloc(0x1000);
    state->spImem = malloc(0x1000);
    memset(state->spDmem, 0, 0x1000);
    memset(state->spImem, 0, 0x1000);
    state->miRegisters = malloc(0x10);
    memset(state->miRegisters, 0, 0x10);
    state->piDmaRegisters = malloc(20);
    memset(state->piDmaRegisters, 0, 20);
    return state;
}

void azExit(AzState* state)
{
    if (state)
    {
        free(state->rdram);
        free(state->cart);
        free(state->spDmem);
        free(state->spImem);
        free(state->miRegisters);
        free(state->piDmaRegisters);
    }
    free(state);
}
