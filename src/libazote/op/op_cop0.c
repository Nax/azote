#include <libazote/libazote.h>

AZOTE_PROTO_COP0(azOpMTC0)
{
    state->cop0.registers[rd] = state->cpu.registers[rt];
}

AZOTE_PROTO_COP0(azOpMFC0)
{
    if (rt)
    {
        state->cpu.registers[rd] = state->cop0.registers[rt];
    }
}
