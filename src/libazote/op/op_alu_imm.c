#include <libazote/libazote.h>

AZOTE_PROTO_I(azOpADDI)
{
    uint32_t result;

    result = (uint32_t)state->cpu.registers[rs] + (uint32_t)((int16_t)imm);
    if (rt)
    {
        state->cpu.registers[rt] = (int32_t)result;
    }
}

AZOTE_PROTO_I(azOpADDIU)
{
    uint32_t result;

    result = (uint32_t)state->cpu.registers[rs] + (uint32_t)((int16_t)imm);
    if (rt)
    {
        state->cpu.registers[rt] = (int32_t)result;
    }
}

AZOTE_PROTO_I(azOpSLTI)
{
    uint64_t result;

    result = (int64_t)state->cpu.registers[rs] < (int64_t)((int16_t)imm);
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpSLTIU)
{
    uint64_t result;

    result = state->cpu.registers[rs] < (uint64_t)((int16_t)imm);
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpANDI)
{
    uint64_t result;

    result = state->cpu.registers[rs] & (uint64_t)imm;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpORI)
{
    uint64_t result;

    result = state->cpu.registers[rs] | (uint64_t)imm;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpXORI)
{
    uint64_t result;

    result = state->cpu.registers[rs] ^ (uint64_t)imm;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpLUI)
{
    int64_t result;

    result = ((int64_t)((int16_t)imm)) << 16;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpDADDI)
{
    int64_t result;

    result = (int64_t)state->cpu.registers[rs] + (int16_t)imm;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}

AZOTE_PROTO_I(azOpDADDIU)
{
    int64_t result;

    result = (int64_t)state->cpu.registers[rs] + (int16_t)imm;
    if (rt)
    {
        state->cpu.registers[rt] = result;
    }
}
