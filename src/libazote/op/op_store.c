#include <libazote/libazote.h>

AZOTE_PROTO_I(azOpSB)
{
    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    azMemoryWrite8(state, addr, state->cpu.registers[rt]);
}

AZOTE_PROTO_I(azOpSH)
{
    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    azMemoryWrite16(state, addr, state->cpu.registers[rt]);
}

AZOTE_PROTO_I(azOpSW)
{
    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    azMemoryWrite32(state, addr, state->cpu.registers[rt]);
}

AZOTE_PROTO_I(azOpSWL)
{
    static const uint32_t masks[] = {
        0xffffffff,
        0x00ffffff,
        0x0000ffff,
        0x000000ff,
    };

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    uint8_t off = addr & 0x3;
    addr &= 0xfffffffffffffffcULL;
    uint32_t value = state->cpu.registers[rt] >> (off * 8);
    uint32_t base = azMemoryRead32(state, addr);
    uint32_t mask = masks[off];
    value = (value & mask) | (base & ~mask);
    azMemoryWrite32(state, addr, value);
}

AZOTE_PROTO_I(azOpSWR)
{
    static const uint32_t masks[] = {
        0xffffffff,
        0xffffff00,
        0xffff0000,
        0xff000000,
    };

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    uint8_t off = addr & 0x3;
    addr &= 0xfffffffffffffffcULL;
    uint32_t value = state->cpu.registers[rt] << (off * 8);
    uint32_t base = azMemoryRead32(state, addr);
    uint32_t mask = masks[off];
    value = (value & mask) | (base & ~mask);
    azMemoryWrite32(state, addr, value);
}

AZOTE_PROTO_I(azOpSD)
{
    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    azMemoryWrite64(state, addr, state->cpu.registers[rt]);
}

AZOTE_PROTO_I(azOpSDL)
{
    static const uint64_t masks[] = {
        0xffffffffffffffff,
        0x00ffffffffffffff,
        0x0000ffffffffffff,
        0x000000ffffffffff,
        0x00000000ffffffff,
        0x0000000000ffffff,
        0x000000000000ffff,
        0x00000000000000ff,
    };

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    uint8_t off = addr & 0x7;
    addr &= 0xfffffffffffffff8ULL;
    uint64_t value = state->cpu.registers[rt] >> (off * 8);
    uint64_t base = azMemoryRead64(state, addr);
    uint64_t mask = masks[off];
    value = (value & mask) | (base & ~mask);
    azMemoryWrite64(state, addr, value);
}

AZOTE_PROTO_I(azOpSDR)
{
    static const uint64_t masks[] = {
        0xffffffffffffffff,
        0xffffffffffffff00,
        0xffffffffffff0000,
        0xffffffffff000000,
        0xffffffff00000000,
        0xffffff0000000000,
        0xffff000000000000,
        0xff00000000000000,
    };

    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    uint8_t off = addr & 0x7;
    addr &= 0xfffffffffffffff8ULL;
    uint64_t value = state->cpu.registers[rt] << (off * 8);
    uint64_t base = azMemoryRead64(state, addr);
    uint64_t mask = masks[off];
    value = (value & mask) | (base & ~mask);
    azMemoryWrite64(state, addr, value);
}

AZOTE_PROTO_I(azOpSC)
{
    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    if (state->cpu.ll)
    {
        azMemoryWrite32(state, addr, state->cpu.registers[rt]);
        if (rt)
        {
            state->cpu.registers[rt] = 1;
        }
    }
    else
        state->cpu.registers[rt] = 0;
}

AZOTE_PROTO_I(azOpSCD)
{
    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    if (state->cpu.ll)
    {
        azMemoryWrite32(state, addr, state->cpu.registers[rt]);
        if (rt)
        {
            state->cpu.registers[rt] = 1;
        }
    }
    else
        state->cpu.registers[rt] = 0;
}

AZOTE_PROTO_I(azOpSWC1)
{
    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    azMemoryWrite32(state, addr, state->cop1.registers[rt].u32);
}

AZOTE_PROTO_I(azOpSDC1)
{
    uint64_t addr = state->cpu.registers[rs] + (int16_t)imm;
    azMemoryWrite64(state, addr, state->cop1.registers[rt].u64);
}
