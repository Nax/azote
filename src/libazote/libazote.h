#ifndef INCLUDED_AZOTE_LIBAZOTE_H
#define INCLUDED_AZOTE_LIBAZOTE_H

/* This file is the private libazote header */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <azote/azote.h>

inline static uint8_t bswap8(uint8_t in)
{
    return in;
}

inline static uint16_t bswap16(uint16_t in)
{
    return ((in << 8) | (in >> 8));
}

inline static uint32_t bswap32(uint32_t in)
{
    uint32_t out = 0;

    out |= ((in << 24) & 0xff000000);
    out |= ((in <<  8) & 0x00ff0000);
    out |= ((in >>  8) & 0x0000ff00);
    out |= ((in >> 24) & 0x000000ff);

    return out;
}

inline static uint64_t bswap64(uint64_t in)
{
    uint64_t out = 0;

    out |= ((in << 56) & 0xff00000000000000ULL);
    out |= ((in << 40) & 0x00ff000000000000ULL);
    out |= ((in << 24) & 0x0000ff0000000000ULL);
    out |= ((in <<  8) & 0x000000ff00000000ULL);
    out |= ((in >>  8) & 0x00000000ff000000ULL);
    out |= ((in >> 24) & 0x0000000000ff0000ULL);
    out |= ((in >> 40) & 0x000000000000ff00ULL);
    out |= ((in >> 56) & 0x00000000000000ffULL);

    return out;
}


void azDebugDumpState(AzState* state);
void azDebugHexdumpRaw(char* addr, size_t len);

#define AZOTE_MEMORY_SIZE   (8 * 1024 * 1024)
#define AZOTE_INST_I        0
#define AZOTE_INST_J        1
#define AZOTE_INST_R        2

#define AZOTE_PROTO_I(x)    void x(AzState* state, uint8_t rs, uint8_t rt, uint16_t imm)

AZOTE_PROTO_I(azOpAddi);
AZOTE_PROTO_I(azOpAddui);
AZOTE_PROTO_I(azOpSlti);
AZOTE_PROTO_I(azOpSltiu);
AZOTE_PROTO_I(azOpAndi);
AZOTE_PROTO_I(azOpOri);
AZOTE_PROTO_I(azOpXori);
AZOTE_PROTO_I(azOpLui);
AZOTE_PROTO_I(azOpDaddi);
AZOTE_PROTO_I(azOpDaddiu);

typedef void (AzProcInstructionI)(AzState* state, uint8_t rs, uint8_t rt, uint16_t imm);
typedef void (AzProcInstructionJ)(AzState* state, uint32_t target);
typedef void (AzProcInstructionR)(AzState* state, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t sa, uint8_t funct);

uint8_t     azMemoryRead8(AzState* state, uint64_t addr);
uint16_t    azMemoryRead16(AzState* state, uint64_t addr);
uint32_t    azMemoryRead32(AzState* state, uint64_t addr);
uint64_t    azMemoryRead64(AzState* state, uint64_t addr);

void azMemoryWrite8(AzState* state, uint64_t addr, uint8_t value);
void azMemoryWrite16(AzState* state, uint64_t addr, uint16_t value);
void azMemoryWrite32(AzState* state, uint64_t addr, uint32_t value);
void azMemoryWrite64(AzState* state, uint64_t addr, uint64_t value);

struct AzCPU_ {
    uint64_t    registers[32];
    uint64_t    pc;
};

struct AzState_ {
    AzCPU       cpu;
    uint64_t    cartSize;
    char*       cart;
    char*       rdram;
    char*       spDmem;
    char*       spImem;
};

#endif
