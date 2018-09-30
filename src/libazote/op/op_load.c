#include <libazote/libazote.h>

AZOTE_PROTO_I(azOpLB)
{
    if (!rt) return;

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    state->cpu.registers[rt] = (int8_t)azMemoryRead8(state, addr);
}

AZOTE_PROTO_I(azOpLBU)
{
    if (!rt) return;

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    state->cpu.registers[rt] = azMemoryRead8(state, addr);
}

AZOTE_PROTO_I(azOpLH)
{
    if (!rt) return;

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    state->cpu.registers[rt] = (int16_t)azMemoryRead16(state, addr);
}

AZOTE_PROTO_I(azOpLHU)
{
    if (!rt) return;

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    state->cpu.registers[rt] = azMemoryRead16(state, addr);
}

AZOTE_PROTO_I(azOpLW)
{
    if (!rt) return;

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    state->cpu.registers[rt] = (int32_t)azMemoryRead32(state, addr);
}

AZOTE_PROTO_I(azOpLWU)
{
    if (!rt) return;

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    state->cpu.registers[rt] = azMemoryRead32(state, addr);
}

AZOTE_PROTO_I(azOpLWL)
{
    static const uint32_t masks[] = {
        0xffffffff,
        0xffffff00,
        0xffff0000,
        0xff000000,
    };

    if (!rt) return;

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    uint8_t off = addr & 0x3;
    addr &= 0xfffffffffffffffcULL;
    uint32_t value = azMemoryRead32(state, addr);
    uint32_t base = state->cpu.registers[rt];
    uint32_t mask = masks[off];

    value <<= (off * 8);
    value = (value & mask) | (base & ~mask);

    state->cpu.registers[rt] = (int64_t)value;
}

AZOTE_PROTO_I(azOpLWR)
{
    static const uint32_t masks[] = {
        0xffffffff,
        0x00ffffff,
        0x0000ffff,
        0x000000ff,
    };

    if (!rt) return;

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    uint8_t off = addr & 0x3;
    addr &= 0xfffffffffffffffcULL;
    uint32_t value = azMemoryRead32(state, addr);
    uint32_t base = state->cpu.registers[rt];
    uint32_t mask = masks[off];

    value >>= (off * 8);
    value = (value & mask) | (base & ~mask);

    state->cpu.registers[rt] = (int64_t)value;
}

AZOTE_PROTO_I(azOpLD)
{
    if (!rt) return;

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    state->cpu.registers[rt] = azMemoryRead64(state, addr);
}

AZOTE_PROTO_I(azOpLDL)
{
    static const uint64_t masks[] = {
        0xffffffffffffffffULL,
        0xffffffffffffff00ULL,
        0xffffffffffff0000ULL,
        0xffffffffff000000ULL,
        0xffffffff00000000ULL,
        0xffffff0000000000ULL,
        0xffff000000000000ULL,
        0xff00000000000000ULL,
    };

    if (!rt) return;

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    uint8_t off = addr & 0x7;
    addr &= 0xfffffffffffffff8ULL;
    uint64_t value = azMemoryRead64(state, addr);
    uint64_t base = state->cpu.registers[rt];
    uint64_t mask = masks[off];

    value <<= (off * 8);
    value = (value & mask) | (base & ~mask);

    state->cpu.registers[rt] = value;
}

AZOTE_PROTO_I(azOpLDR)
{
    static const uint64_t masks[] = {
        0xffffffffffffffffULL,
        0x00ffffffffffffffULL,
        0x0000ffffffffffffULL,
        0x000000ffffffffffULL,
        0x00000000ffffffffULL,
        0x0000000000ffffffULL,
        0x000000000000ffffULL,
        0x00000000000000ffULL,
    };

    if (!rt) return;

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    uint8_t off = addr & 0x7;
    addr &= 0xfffffffffffffff8ULL;
    uint64_t value = azMemoryRead64(state, addr);
    uint64_t base = state->cpu.registers[rt];
    uint64_t mask = masks[off];

    value >>= (off * 8);
    value = (value & mask) | (base & ~mask);

    state->cpu.registers[rt] = value;
}
