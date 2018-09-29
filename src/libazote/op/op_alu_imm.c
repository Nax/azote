#include <libazote/libazote.h>

AZOTE_PROTO_I(azOpAddi)
{
    int32_t result;

    result = (int32_t)state->cpu.registers[rs] + (int16_t)imm;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpAddiu)
{
    int32_t result;

    result = (int32_t)state->cpu.registers[rs] + (int16_t)imm;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpSlti)
{
    uint64_t result;

    result = (int64_t)state->cpu.registers[rs] < (int64_t)((int16_t)imm);
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpSltiu)
{
    uint64_t result;

    result = state->cpu.registers[rs] < (uint64_t)((int16_t)imm);
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpAndi)
{
    uint64_t result;

    result = state->cpu.registers[rs] & (uint64_t)imm;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpOri)
{
    uint64_t result;

    result = state->cpu.registers[rs] | (uint64_t)imm;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpXori)
{
    uint64_t result;

    result = state->cpu.registers[rs] ^ (uint64_t)imm;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpLui)
{
    int64_t result;

    result = ((int64_t)((int16_t)imm)) << 16;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpDaddi)
{
    int64_t result;

    result = (int64_t)state->cpu.registers[rs] + (int16_t)imm;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpDaddiu)
{
    int64_t result;

    result = (int64_t)state->cpu.registers[rs] + (int16_t)imm;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}
