#include <libazote/libazote.h>

AZOTE_PROTO_R(azOpMFLO)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.lo;
}

AZOTE_PROTO_R(azOpMFHI)
{
    if (!rd) return;

    state->cpu.registers[rd] = state->cpu.hi;
}

AZOTE_PROTO_R(azOpMTLO)
{
    state->cpu.lo = state->cpu.registers[rs];
}

AZOTE_PROTO_R(azOpMTHI)
{
    state->cpu.hi = state->cpu.registers[rs];
}

AZOTE_PROTO_R(azOpMULT)
{
    int32_t a;
    int32_t b;
    int64_t result;

    a = state->cpu.registers[rs];
    b = state->cpu.registers[rt];
    result = (int64_t)a * (int64_t)b;

    uint32_t hi = (uint64_t)result >> 32;
    uint32_t lo = (uint64_t)result & 0xffffffff;
    state->cpu.hi = (int32_t)hi;
    state->cpu.lo = (int32_t)lo;
}

AZOTE_PROTO_R(azOpMULTU)
{
    uint32_t a;
    uint32_t b;
    uint64_t result;

    a = state->cpu.registers[rs];
    b = state->cpu.registers[rt];
    result = (uint64_t)a * (uint64_t)b;

    uint32_t hi = (uint64_t)result >> 32;
    uint32_t lo = (uint64_t)result & 0xffffffff;
    state->cpu.hi = (int32_t)hi;
    state->cpu.lo = (int32_t)lo;
}

AZOTE_PROTO_R(azOpDMULT)
{
    int64_t a;
    int64_t b;
    __int128_t result;

    a = state->cpu.registers[rs];
    b = state->cpu.registers[rt];
    result = (__int128_t)a * (__int128_t)b;

    state->cpu.hi = (__uint128_t)result >> 64;
    state->cpu.lo = (__uint128_t)result & 0xffffffffffffffffULL;
}

AZOTE_PROTO_R(azOpDMULTU)
{
    int64_t a;
    int64_t b;
    __uint128_t result;

    a = state->cpu.registers[rs];
    b = state->cpu.registers[rt];
    result = (__uint128_t)a * (__uint128_t)b;

    state->cpu.hi = (__uint128_t)result >> 64;
    state->cpu.lo = (__uint128_t)result & 0xffffffffffffffffULL;
}

AZOTE_PROTO_R(azOpDIV)
{
    int32_t a;
    int32_t b;

    a = (uint32_t)state->cpu.registers[rs];
    b = (uint32_t)state->cpu.registers[rt];
    state->cpu.lo = a / b;
    state->cpu.hi = a % b;
}

AZOTE_PROTO_R(azOpDIVU)
{
    uint32_t a;
    uint32_t b;

    a = state->cpu.registers[rs];
    b = state->cpu.registers[rt];
    state->cpu.lo = (int32_t)(a / b);
    state->cpu.hi = (int32_t)(a % b);
}

AZOTE_PROTO_R(azOpDDIV)
{
    int64_t a;
    int64_t b;

    a = state->cpu.registers[rs];
    b = state->cpu.registers[rt];
    state->cpu.lo = a / b;
    state->cpu.hi = a % b;
}

AZOTE_PROTO_R(azOpDDIVU)
{
    uint64_t a;
    uint64_t b;

    a = state->cpu.registers[rs];
    b = state->cpu.registers[rt];
    state->cpu.lo = a / b;
    state->cpu.hi = a % b;
}
