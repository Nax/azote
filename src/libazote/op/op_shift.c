#include <libazote/libazote.h>

static uint32_t _sra32(uint32_t x, uint8_t shift)
{
    return (x >> shift) | -((x & ~(0xffffffffU >> 1)) >> shift);
}

static uint64_t _sra64(uint64_t x, uint8_t shift)
{
    return (x >> shift) | -((x & ~(0xffffffffffffffffULL >> 1)) >> shift);
}

AZOTE_PROTO_R(azOpSLL)
{
    if (!rd) return;

    uint32_t value = (uint32_t)state->cpu.registers[rt] << sa;
    state->cpu.registers[rd] = (int32_t)value;
}

AZOTE_PROTO_R(azOpSRL)
{
    if (!rd) return;

    uint32_t value = (uint32_t)state->cpu.registers[rt] >> sa;
    state->cpu.registers[rd] = (int32_t)value;
}

AZOTE_PROTO_R(azOpSRA)
{
    if (!rd) return;

    uint32_t value = _sra32(state->cpu.registers[rt], sa);
    state->cpu.registers[rd] = (int32_t)value;
}

AZOTE_PROTO_R(azOpSLLV)
{
    if (!rd) return;

    uint32_t value = (uint32_t)state->cpu.registers[rt] << (state->cpu.registers[rs] & 0x1f);
    state->cpu.registers[rd] = (int32_t)value;
}

AZOTE_PROTO_R(azOpSRLV)
{
    if (!rd) return;

    uint32_t value = (uint32_t)state->cpu.registers[rt] >> (state->cpu.registers[rs] & 0x1f);
    state->cpu.registers[rd] = (int32_t)value;
}

AZOTE_PROTO_R(azOpSRAV)
{
    if (!rd) return;

    uint32_t value = _sra32(state->cpu.registers[rt], state->cpu.registers[rs] & 0x1f);
    state->cpu.registers[rd] = (int32_t)value;
}

AZOTE_PROTO_R(azOpDSLL)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rt] << sa;
}

AZOTE_PROTO_R(azOpDSRL)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rt] >> sa;
}

AZOTE_PROTO_R(azOpDSRA)
{
    if (!rd) return;

    state->cpu.registers[rd] = _sra64(state->cpu.registers[rt], sa);
}

AZOTE_PROTO_R(azOpDSLLV)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rt] << (state->cpu.registers[rs] & 0x3f);
}

AZOTE_PROTO_R(azOpDSRLV)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rt] >> (state->cpu.registers[rs] & 0x3f);
}

AZOTE_PROTO_R(azOpDSRAV)
{
    if (!rd) return;

    state->cpu.registers[rd] = _sra64(state->cpu.registers[rt], state->cpu.registers[rs] & 0x3f);
}

AZOTE_PROTO_R(azOpDSLL32)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rt] << (sa + 32);
}

AZOTE_PROTO_R(azOpDSRL32)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.registers[rt] >> (sa + 32);
}

AZOTE_PROTO_R(azOpDSRA32)
{
    if (!rd) return;

    state->cpu.registers[rd] = _sra64(state->cpu.registers[rt], sa + 32);
}
