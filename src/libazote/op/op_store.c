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
