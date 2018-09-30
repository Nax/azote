#include <libazote/libazote.h>

AZOTE_PROTO_COP(azOpMTC1)
{
    state->cop1.registers[rd].u32 = state->cpu.registers[rt];
}

AZOTE_PROTO_COP(azOpMFC1)
{
    if (!rt) return;

    state->cpu.registers[rt] = state->cop1.registers[rd].i32;
}

AZOTE_PROTO_COP(azOpCTC1)
{
    if (rd != 31) return;

    state->cop1.fcr31 = state->cpu.registers[rt];
}

AZOTE_PROTO_COP(azOpCFC1)
{
    if (!rt) return;

    switch (rd)
    {
    case 0:
        state->cpu.registers[rt] = state->cop1.fcr0;
        return;
    case 31:
        state->cpu.registers[rt] = state->cop1.fcr31;
        return;
    }
}

AZOTE_PROTO_COP(azOpDMTC1)
{
    state->cop1.registers[rd].u64 = state->cpu.registers[rt];
}

AZOTE_PROTO_COP(azOpDMFC1)
{
    if (!rt) return;

    state->cpu.registers[rt] = state->cop1.registers[rd].u64;
}
