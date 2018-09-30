#include <libazote/libazote.h>

AZOTE_PROTO_J(azOpJ)
{
    state->cpu.pc2 = (state->cpu.pc & 0xfffffffff0000000) | (target << 2);
}

AZOTE_PROTO_J(azOpJAL)
{
    state->cpu.registers[31] = state->cpu.pc + 4;
    state->cpu.pc2 = (state->cpu.pc & 0xfffffffff0000000) | (target << 2);
}

AZOTE_PROTO_R(azOpJR)
{
    state->cpu.pc2 = state->cpu.registers[rs];
}

AZOTE_PROTO_R(azOpJALR)
{
    uint64_t tmp = state->cpu.registers[rs];
    if (rd)
    {
        state->cpu.registers[rd] = state->cpu.pc + 4;
    }
    state->cpu.pc2 = tmp;
}

AZOTE_PROTO_I(azOpBEQ)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    if (state->cpu.registers[rs] == state->cpu.registers[rt])
        state->cpu.pc2 = addr;
}

AZOTE_PROTO_I(azOpBEQL)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    if (state->cpu.registers[rs] == state->cpu.registers[rt])
        state->cpu.pc2 = addr;
    else
    {
        state->cpu.pc += 4;
        state->cpu.pc2 += 4;
    }
}

AZOTE_PROTO_REGIMM(azOpBEGZ)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    if (state->cpu.registers[rs] >= 0)
        state->cpu.pc2 = addr;
}

AZOTE_PROTO_REGIMM(azOpBEGZL)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    if (state->cpu.registers[rs] >= 0)
        state->cpu.pc2 = addr;
    else
    {
        state->cpu.pc += 4;
        state->cpu.pc2 += 4;
    }
}

AZOTE_PROTO_REGIMM(azOpBEGZAL)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    state->cpu.registers[31] = state->cpu.pc + 4;
    if (state->cpu.registers[rs] >= 0)
        state->cpu.pc2 = addr;
}

AZOTE_PROTO_REGIMM(azOpBEGZALL)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    state->cpu.registers[31] = state->cpu.pc + 4;
    if (state->cpu.registers[rs] >= 0)
        state->cpu.pc2 = addr;
    else
    {
        state->cpu.pc += 4;
        state->cpu.pc2 += 4;
    }
}

AZOTE_PROTO_I(azOpBGTZ)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    if (state->cpu.registers[rs] > 0)
        state->cpu.pc2 = addr;
}

AZOTE_PROTO_I(azOpBGTZL)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    if (state->cpu.registers[rs] > 0)
        state->cpu.pc2 = addr;
    else
    {
        state->cpu.pc += 4;
        state->cpu.pc2 += 4;
    }
}

AZOTE_PROTO_I(azOpBLEZ)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    if (state->cpu.registers[rs] <= 0)
        state->cpu.pc2 = addr;
}

AZOTE_PROTO_I(azOpBLEZL)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    if (state->cpu.registers[rs] <= 0)
        state->cpu.pc2 = addr;
    else
    {
        state->cpu.pc += 4;
        state->cpu.pc2 += 4;
    }
}

AZOTE_PROTO_REGIMM(azOpBLTZ)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    if (state->cpu.registers[rs] < 0)
        state->cpu.pc2 = addr;
}

AZOTE_PROTO_REGIMM(azOpBLTZL)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    if (state->cpu.registers[rs] < 0)
        state->cpu.pc2 = addr;
    else
    {
        state->cpu.pc += 4;
        state->cpu.pc2 += 4;
    }
}

AZOTE_PROTO_REGIMM(azOpBLTZAL)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    state->cpu.registers[31] = state->cpu.pc + 4;
    if (state->cpu.registers[rs] < 0)
        state->cpu.pc2 = addr;
}

AZOTE_PROTO_REGIMM(azOpBLTZALL)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    state->cpu.registers[31] = state->cpu.pc + 4;
    if (state->cpu.registers[rs] < 0)
        state->cpu.pc2 = addr;
    else
    {
        state->cpu.pc += 4;
        state->cpu.pc2 += 4;
    }
}

AZOTE_PROTO_I(azOpBNE)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    if (state->cpu.registers[rs] != state->cpu.registers[rt])
        state->cpu.pc2 = addr;
}

AZOTE_PROTO_I(azOpBNEL)
{
    uint64_t addr = state->cpu.pc + ((int16_t)imm << 2);

    if (state->cpu.registers[rs] != state->cpu.registers[rt])
        state->cpu.pc2 = addr;
    else
    {
        state->cpu.pc += 4;
        state->cpu.pc2 += 4;
    }
}
