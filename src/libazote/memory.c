#include <stdio.h>
#include <libazote/libazote.h>

static uint64_t _badIO(AzState* state, uint64_t addr, int write, int align)
{
    printf("*** Bad io (%s%s) at: 0x%016llx   PC: 0x%016llx\n", (write ? "W" : "R"), (align ? ", unaligned" : ""), addr, state->cpu.pc);
    state->debug = 1;
    getchar();
    return 0;
}

static uint64_t _removeSegment(uint64_t addr)
{
    addr &= 0xffffffff;
    if ((addr & 0x80000000) == 0)
    {
        return addr;
    }
    return addr & 0x1fffffff;
}

#define READ_MEMORY(x, type, swap)                                      \
type x(AzState* state, uint64_t addr)                                   \
{                                                                       \
    type res;                                                           \
    addr = _removeSegment(addr);                                        \
    if (addr % sizeof(type) != 0)                                       \
        return _badIO(state, addr, 0, 1);                               \
    if (addr >= 0x00000000 && addr < AZOTE_MEMORY_SIZE)                 \
        res = swap(*(type*)(state->rdram + addr));                      \
    else if (addr >= 0x03f00000 && addr < 0x03f00028)                   \
        res = swap(*(type*)(state->rdramRegisters + (addr & 0xff)));    \
    else if (addr >= 0x03f00028 && addr < 0x04000000)                   \
        res = 0;                                                        \
    else if (addr >= 0x04000000 && addr < 0x04001000)                   \
        res = swap(*(type*)(state->spDmem + (addr & 0xfff)));           \
    else if (addr >= 0x04001000 && addr < 0x04002000)                   \
        res = swap(*(type*)(state->spImem + (addr & 0xfff)));           \
    else if (addr >= 0x04040000 && addr < 0x04040020)                   \
        res = swap(*(type*)(state->sp1Registers + (addr & 0xff)));      \
    else if (addr >= 0x04080000 && addr < 0x04080008)                   \
        res = swap(*(type*)(state->sp2Registers + (addr & 0xff)));      \
    else if (addr >= 0x04300000 && addr < 0x04300010)                   \
        res = azRcpReadMI(state, addr);                                 \
    else if (addr >= 0x04400000 && addr < 0x04400038)                   \
        res = swap(*(type*)(state->viRegisters + (addr & 0xff)));       \
    else if (addr >= 0x04500000 && addr < 0x04500018)                   \
        res = swap(*(type*)(state->aiRegisters + (addr & 0xff)));       \
    else if (addr >= 0x04600000 && addr < 0x04600034)                   \
        res = azRcpReadPI(state, addr);                                 \
    else if (addr >= 0x04700000 && addr < 0x04700020)                   \
            res = swap(*(type*)(state->piRegisters + (addr & 0xff)));   \
    else if (addr >= 0x04800000 && addr < 0x0480001c)                   \
        res = swap(*(type*)(state->siRegisters + (addr & 0xff)));       \
    else if (addr >= 0x10000000 && addr < 0x10000000 + state->cartSize) \
        res = swap(*(type*)(state->cart + (addr & 0xfffffff)));         \
    else if (addr >= 0x1fc007c0 && addr < 0x1fc00800)                   \
        res = swap(*(type*)(state->pifram + ((addr & 0xff) - 0xc0)));   \
    else                                                                \
        res = _badIO(state, addr, 0, 0);                                \
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
    if (addr % sizeof(type) != 0)                                       \
    {                                                                   \
        _badIO(state, addr, 0, 1);                                      \
        return;                                                         \
    }                                                                   \
    if (addr >= 0x00000000 && addr < AZOTE_MEMORY_SIZE)                 \
        *(type*)(state->rdram + addr) = swap(value);                    \
    else if (addr >= 0x03f00000 && addr < 0x03f00028)                   \
        *(type*)(state->rdramRegisters + (addr & 0xff)) = swap(value);  \
    else if (addr >= 0x03f00028 && addr < 0x04000000)                   \
        ; /* no-op */                                                   \
    else if (addr >= 0x04000000 && addr < 0x04001000)                   \
        *(type*)(state->spDmem + (addr & 0xfff)) = swap(value);         \
    else if (addr >= 0x04001000 && addr < 0x04002000)                   \
        *(type*)(state->spImem + (addr & 0xfff)) = swap(value);         \
    else if (addr >= 0x04040000 && addr < 0x04040020)                   \
        *(type*)(state->sp1Registers + (addr & 0xff)) = swap(value);    \
    else if (addr >= 0x04080000 && addr < 0x04080008)                   \
        *(type*)(state->sp2Registers + (addr & 0xff)) = swap(value);    \
    else if (addr >= 0x04300000 && addr < 0x04300010)                   \
        azRcpWriteMI(state, addr, value);                               \
    else if (addr >= 0x04400000 && addr < 0x04400038)                   \
        *(type*)(state->viRegisters + (addr & 0xff)) = swap(value);     \
    else if (addr >= 0x04500000 && addr < 0x04500018)                   \
        *(type*)(state->aiRegisters + (addr & 0xff)) = swap(value);     \
    else if (addr >= 0x04600000 && addr < 0x04600034)                   \
        azRcpWritePI(state, addr, value);                               \
    else if (addr >= 0x04700000 && addr < 0x04700020)                   \
        *(type*)(state->riRegisters + (addr & 0xff)) = swap(value);     \
    else if (addr >= 0x04800000 && addr < 0x0480001c)                   \
        *(type*)(state->siRegisters + (addr & 0xff)) = swap(value);     \
    else if (addr >= 0x1fc007c0 && addr < 0x1fc00800)                   \
        *(type*)(state->pifram + ((addr & 0xff - 0xc0))) = swap(value); \
    else                                                                \
        _badIO(state, addr, 1, 0);                                      \
}

WRITE_MEMORY(azMemoryWrite8, uint8_t, bswap8);
WRITE_MEMORY(azMemoryWrite16, uint16_t, bswap16);
WRITE_MEMORY(azMemoryWrite32, uint32_t, bswap32);
WRITE_MEMORY(azMemoryWrite64, uint64_t, bswap64);
