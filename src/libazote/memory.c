#include <libazote/libazote.h>

#define READ_MEMORY(state, addr, type, swap)                    \
({  type res;                                                   \
    __typeof__ (addr) _addr = _removeSegment(addr);             \
    if (_addr < AZOTE_MEMORY_SIZE)                              \
        res = swap(*(type*)(state->rdram + _addr));             \
    else if (_addr < 0x04000000)                                \
        res = 0;                                                \
    else if (_addr < 0x04001000)                                \
        res = swap(*(type*)(state->spDmem + (_addr & 0xfff)));  \
    else if (_addr < 0x04002000)                                \
        res = swap(*(type*)(state->spImem + (_addr & 0xfff)));  \
    else if (_addr < 0x10000000)                                \
        res = 0;                                                \
    else if (_addr < 0x10000000 + state->cartSize)              \
        res = swap(*(type*)(state->cart + (_addr & 0xfffffff)));\
    else                                                        \
        res = 0;                                                \
    res; })

static uint64_t _removeSegment(uint64_t addr)
{
    addr &= 0xffffffff;
    if ((addr & 0x80000000) == 0)
    {
        return addr;
    }
    return addr & 0x1fffffff;
}

uint8_t azMemoryRead8(AzState* state, uint64_t addr)
{
    return READ_MEMORY(state, addr, uint8_t, bswap8);
}

uint16_t azMemoryRead16(AzState* state, uint64_t addr)
{
    return READ_MEMORY(state, addr, uint16_t, bswap16);
}

uint32_t azMemoryRead32(AzState* state, uint64_t addr)
{
    return READ_MEMORY(state, addr, uint32_t, bswap32);
}

uint64_t azMemoryRead64(AzState* state, uint64_t addr)
{
    return READ_MEMORY(state, addr, uint64_t, bswap64);
}

void azMemoryWrite8(AzState* state, uint64_t addr, uint8_t value)
{

}

void azMemoryWrite16(AzState* state, uint64_t addr, uint16_t value)
{

}

void azMemoryWrite32(AzState* state, uint64_t addr, uint32_t value)
{

}

void azMemoryWrite64(AzState* state, uint64_t addr, uint64_t value)
{

}
