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

inline static uint64_t sext8(uint8_t v)
{
    return ((int64_t)((int8_t)v));
}

inline static uint64_t sext16(uint16_t v)
{
    return ((int64_t)((int16_t)v));
}

inline static uint64_t sext32(uint32_t v)
{
    return ((int64_t)((int32_t)v));
}


void azDebugDumpState(AzState* state);
void azDebugHexdumpRaw(char* addr, size_t len);

#define AZOTE_MEMORY_SIZE   (8 * 1024 * 1024)
#define AZOTE_INST_NONE     0
#define AZOTE_INST_I        1
#define AZOTE_INST_J        2
#define AZOTE_INST_R        3

#define AZOTE_PROTO_I(x)        void x(AzState* state, uint8_t rs, uint8_t rt, uint16_t imm)
#define AZOTE_PROTO_J(x)        void x(AzState* state, uint32_t target)
#define AZOTE_PROTO_R(x)        void x(AzState* state, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t sa)
#define AZOTE_PROTO_REGIMM(x)   void x(AzState* state, uint8_t rs, uint16_t imm)
#define AZOTE_PROTO_COP0(x)     void x(AzState* state, uint8_t rt, uint8_t rd)

AZOTE_PROTO_I(azOpADDI);
AZOTE_PROTO_I(azOpADDIU);
AZOTE_PROTO_I(azOpSLTI);
AZOTE_PROTO_I(azOpSLTIU);
AZOTE_PROTO_I(azOpANDI);
AZOTE_PROTO_I(azOpORI);
AZOTE_PROTO_I(azOpXORI);
AZOTE_PROTO_I(azOpLUI);
AZOTE_PROTO_I(azOpDADDI);
AZOTE_PROTO_I(azOpDADDIU);

AZOTE_PROTO_I(azOpLB);
AZOTE_PROTO_I(azOpLBU);
AZOTE_PROTO_I(azOpLH);
AZOTE_PROTO_I(azOpLHU);
AZOTE_PROTO_I(azOpLW);
AZOTE_PROTO_I(azOpLWU);
AZOTE_PROTO_I(azOpLWL);
AZOTE_PROTO_I(azOpLWR);
AZOTE_PROTO_I(azOpLD);
AZOTE_PROTO_I(azOpLDL);
AZOTE_PROTO_I(azOpLDR);

AZOTE_PROTO_I(azOpSB);
AZOTE_PROTO_I(azOpSH);
AZOTE_PROTO_I(azOpSW);

AZOTE_PROTO_J(azOpJ);
AZOTE_PROTO_J(azOpJAL);
AZOTE_PROTO_R(azOpJR);
AZOTE_PROTO_R(azOpJALR);
AZOTE_PROTO_I(azOpBEQ);
AZOTE_PROTO_I(azOpBEQL);
AZOTE_PROTO_REGIMM(azOpBEGZ);
AZOTE_PROTO_REGIMM(azOpBEGZL);
AZOTE_PROTO_REGIMM(azOpBEGZAL);
AZOTE_PROTO_REGIMM(azOpBEGZALL);
AZOTE_PROTO_I(azOpBGTZ);
AZOTE_PROTO_I(azOpBGTZL);
AZOTE_PROTO_I(azOpBLEZ);
AZOTE_PROTO_I(azOpBLEZL);
AZOTE_PROTO_REGIMM(azOpBLTZ);
AZOTE_PROTO_REGIMM(azOpBLTZL);
AZOTE_PROTO_REGIMM(azOpBLTZAL);
AZOTE_PROTO_REGIMM(azOpBLTZALL);
AZOTE_PROTO_I(azOpBNE);
AZOTE_PROTO_I(azOpBNEL);

AZOTE_PROTO_R(azOpSLL);
AZOTE_PROTO_R(azOpSRL);
AZOTE_PROTO_R(azOpSRA);
AZOTE_PROTO_R(azOpSLLV);
AZOTE_PROTO_R(azOpSRLV);
AZOTE_PROTO_R(azOpSRAV);
AZOTE_PROTO_R(azOpDSLL);
AZOTE_PROTO_R(azOpDSRL);
AZOTE_PROTO_R(azOpDSRA);
AZOTE_PROTO_R(azOpDSLLV);
AZOTE_PROTO_R(azOpDSRLV);
AZOTE_PROTO_R(azOpDSRAV);
AZOTE_PROTO_R(azOpDSLL32);
AZOTE_PROTO_R(azOpDSRL32);
AZOTE_PROTO_R(azOpDSRA32);

AZOTE_PROTO_R(azOpADD);
AZOTE_PROTO_R(azOpADDU);
AZOTE_PROTO_R(azOpSUB);
AZOTE_PROTO_R(azOpSUBU);
AZOTE_PROTO_R(azOpSLT);
AZOTE_PROTO_R(azOpSLTU);
AZOTE_PROTO_R(azOpAND);
AZOTE_PROTO_R(azOpOR);
AZOTE_PROTO_R(azOpXOR);
AZOTE_PROTO_R(azOpNOR);
AZOTE_PROTO_R(azOpDADD);
AZOTE_PROTO_R(azOpDADDU);
AZOTE_PROTO_R(azOpDSUB);
AZOTE_PROTO_R(azOpDSUBU);

AZOTE_PROTO_R(azOpMFLO);
AZOTE_PROTO_R(azOpMFHI);
AZOTE_PROTO_R(azOpMTLO);
AZOTE_PROTO_R(azOpMTHI);
AZOTE_PROTO_R(azOpMULT);
AZOTE_PROTO_R(azOpMULTU);
AZOTE_PROTO_R(azOpDMULT);
AZOTE_PROTO_R(azOpDMULTU);
AZOTE_PROTO_R(azOpDIV);
AZOTE_PROTO_R(azOpDIVU);
AZOTE_PROTO_R(azOpDDIV);
AZOTE_PROTO_R(azOpDDIVU);

AZOTE_PROTO_COP0(azOpMTC0);
AZOTE_PROTO_COP0(azOpMFC0);

typedef void (AzProcInstructionI)(AzState* state, uint8_t rs, uint8_t rt, uint16_t imm);
typedef void (AzProcInstructionJ)(AzState* state, uint32_t target);
typedef void (AzProcInstructionR)(AzState* state, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t sa);
typedef void (AzProcInstructionCOP0)(AzState* state, uint8_t rt, uint8_t rd);

uint8_t     azMemoryRead8(AzState* state, uint64_t addr);
uint16_t    azMemoryRead16(AzState* state, uint64_t addr);
uint32_t    azMemoryRead32(AzState* state, uint64_t addr);
uint64_t    azMemoryRead64(AzState* state, uint64_t addr);

void azMemoryWrite8(AzState* state, uint64_t addr, uint8_t value);
void azMemoryWrite16(AzState* state, uint64_t addr, uint16_t value);
void azMemoryWrite32(AzState* state, uint64_t addr, uint32_t value);
void azMemoryWrite64(AzState* state, uint64_t addr, uint64_t value);

typedef struct AzCPU_   AzCPU;
typedef struct AzCOP0_  AzCOP0;

struct AzCPU_ {
    uint64_t    registers[32];
    uint64_t    hi;
    uint64_t    lo;
    uint64_t    pc;
    uint64_t    pc2;
};

struct AzCOP0_ {
    uint64_t    registers[32];
};

struct AzState_ {
    AzCPU       cpu;
    AzCOP0      cop0;
    uint64_t    cartSize;
    char*       cart;
    char*       rdram;
    char*       spDmem;
    char*       spImem;
};

#endif
