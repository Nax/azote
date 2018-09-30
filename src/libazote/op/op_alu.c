#include <libazote/libazote.h>

AZOTE_PROTO_R(azOpADD)
{
    if (!rd) return;

    uint32_t value = state->cpu.registers[rs] + state->cpu.registers[rt];
    state->cpu.registers[rd] = (int32_t)value;
}

AZOTE_PROTO_R(azOpADDU)
{
    if (!rd) return;

    uint32_t value = state->cpu.registers[rs] + state->cpu.registers[rt];
    state->cpu.registers[rd] = (int32_t)value;
}

AZOTE_PROTO_R(azOpSUB)
{
    if (!rd) return;

    uint32_t value = (uint32_t)state->cpu.registers[rs] - (uint32_t)state->cpu.registers[rt];
    state->cpu.registers[rd] = (int32_t)value;
}

AZOTE_PROTO_R(azOpSUBU)
{
    if (!rd) return;

    uint32_t value = (uint32_t)state->cpu.registers[rs] - (uint32_t)state->cpu.registers[rt];
    state->cpu.registers[rd] = (int32_t)value;
}

AZOTE_PROTO_R(azOpSLT)
{
    if (!rd) return;

    state->cpu.registers[rd] = (int64_t)state->cpu.registers[rs] < (int64_t)state->cpu.registers[rt] ? 1 : 0;
}

AZOTE_PROTO_R(azOpSLTU)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rs] < state->cpu.registers[rt] ? 1 : 0;
}

AZOTE_PROTO_R(azOpAND)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rs] & state->cpu.registers[rt];
}

AZOTE_PROTO_R(azOpOR)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rs] | state->cpu.registers[rt];
}

AZOTE_PROTO_R(azOpXOR)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rs] ^ state->cpu.registers[rt];
}

AZOTE_PROTO_R(azOpNOR)
{
    if (!rd) return;

    state->cpu.registers[rd] = ~(state->cpu.registers[rs] | state->cpu.registers[rt]);
}

AZOTE_PROTO_R(azOpDADD)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rs] + state->cpu.registers[rt];
}

AZOTE_PROTO_R(azOpDADDU)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rs] + state->cpu.registers[rt];
}

AZOTE_PROTO_R(azOpDSUB)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rs] - state->cpu.registers[rt];
}

AZOTE_PROTO_R(azOpDSUBU)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rs] - state->cpu.registers[rt];
}
