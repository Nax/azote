#include <libazote/libazote.h>

static uint64_t _removeSegment(uint64_t addr)
{
    addr &= 0xffffffff;
    if ((addr & 0x80000000) == 0)
    {
        return addr;
    }
    return addr & 0x1fffffff;
}

static void _ramDMA(AzState* state)
{
    uint64_t size;
    uint32_t ramAddr;
    uint32_t romAddr;

    size = (uint64_t)bswap32(*(uint32_t*)(state->piDmaRegisters + 0x0C)) + 1;
    ramAddr = bswap32(*(uint32_t*)(state->piDmaRegisters + 0x00));
    romAddr = bswap32(*(uint32_t*)(state->piDmaRegisters + 0x04));
    printf("DMA (Size: 0x%016llx, 0x%08x -> 0x%08x) !\n", size, romAddr, ramAddr);
    getchar();
    memcpy(state->rdram + ramAddr, state->cart + (romAddr & 0x0fffffff), size);
}

#define READ_MEMORY(x, type, swap)                                      \
type x(AzState* state, uint64_t addr)                                   \
{                                                                       \
    type res;                                                           \
    addr = _removeSegment(addr);                                        \
    if (addr < AZOTE_MEMORY_SIZE)                                       \
        res = swap(*(type*)(state->rdram + addr));                      \
    else if (addr < 0x04000000)                                         \
        res = 0;                                                        \
    else if (addr < 0x04001000)                                         \
        res = swap(*(type*)(state->spDmem + (addr & 0xfff)));           \
    else if (addr < 0x04002000)                                         \
        res = swap(*(type*)(state->spImem + (addr & 0xfff)));           \
    else if (addr < 0x04300004)                                         \
        res = 0;                                                        \
    else if (addr < 0x04300008)                                         \
        res = 0x01010101;                                               \
    else if (addr < 0x04600000)                                         \
        res = 0;                                                        \
    else if (addr < 0x04600014)                                         \
        res = swap(*(type*)(state->piDmaRegisters + (addr & 0xff)));    \
    else if (addr < 0x10000000)                                         \
        res = 0;                                                        \
    else if (addr < 0x10000000 + state->cartSize)                       \
        res = swap(*(type*)(state->cart + (addr & 0xfffffff)));         \
    else                                                                \
        res = 0;                                                        \
    return res;                                                         \
}

READ_MEMORY(azMemoryRead8, uint8_t, bswap8);
READ_MEMORY(azMemoryRead16, uint16_t, bswap16);
READ_MEMORY(azMemoryRead32, uint32_t, bswap32);
READ_MEMORY(azMemoryRead64, uint64_t, bswap64);

#define WRITE_MEMORY(x, type, swap)                                     \
void x(AzState* state, uint64_t addr, type value)                       \
{                                                                       \
    addr = _removeSegment(addr);                                        \
    if (addr < AZOTE_MEMORY_SIZE)                                       \
        *(type*)(state->rdram + addr) = swap(value);                    \
    else if (addr < 0x04000000)                                         \
        return;                                                         \
    else if (addr < 0x04001000)                                         \
        *(type*)(state->spDmem + (addr & 0xfff)) = swap(value);         \
    else if (addr < 0x04002000)                                         \
        *(type*)(state->spImem + (addr & 0xfff)) = swap(value);         \
    else if (addr < 0x04600000)                                         \
        return;                                                         \
    else if (addr < 0x04600010)                                         \
    {                                                                   \
        *(type*)(state->piDmaRegisters + (addr & 0xff)) = swap(value);  \
        if (addr == 0x0460000c) _ramDMA(state);                         \
    }                                                                   \
}

WRITE_MEMORY(azMemoryWrite8, uint8_t, bswap8);
WRITE_MEMORY(azMemoryWrite16, uint16_t, bswap16);
WRITE_MEMORY(azMemoryWrite32, uint32_t, bswap32);
WRITE_MEMORY(azMemoryWrite64, uint64_t, bswap64);
