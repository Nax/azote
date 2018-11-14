#include <libazote/libazote.h>

#define zalloc(x)   calloc((x), 1)

AzState* azInit()
{
    AzState* state;

    state = zalloc(sizeof(*state));

    state->rdram = zalloc(AZOTE_MEMORY_SIZE);
    state->rdramRegisters = zalloc(0x28);
    state->spDmem = zalloc(0x1000);
    state->spImem = zalloc(0x1000);
    state->miRegisters = zalloc(0x10);
    state->viRegisters = zalloc(0x38);
    state->aiRegisters = zalloc(0x18);
    state->piRegisters = zalloc(0x34);
    state->riRegisters = zalloc(0x20);
    state->siRegisters = zalloc(0x1c);
    state->pifram = zalloc(0x40);

    return state;
}

void azExit(AzState* state)
{
    if (state)
    {
        free(state->cart);
        free(state->rdram);
        free(state->rdramRegisters);
        free(state->spDmem);
        free(state->spImem);
        free(state->miRegisters);
        free(state->viRegisters);
        free(state->aiRegisters);
        free(state->piRegisters);
        free(state->riRegisters);
        free(state->siRegisters);
        free(state->pifram);
    }
    free(state);
}
