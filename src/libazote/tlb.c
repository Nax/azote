#include <libazote/libazote.h>

uint32_t _lookupTLB(AzState* state, uint32_t vaddr)
{
    //printf("TLB, vaddr: 0x%08x\n", vaddr);
    //printf("PC:  0x%016llx\n", state->cpu.pc);
    //getchar();
    return vaddr;
}

uint32_t azPhysicalAddress(AzState* state, uint64_t vaddr)
{
    uint32_t vaddr32 = vaddr & 0xffffffff;
    if (vaddr32 & 0x80000000)
    {
        // KSEG0 - KSEG3
        if (vaddr32 & 0x40000000)
        {
            return _lookupTLB(state, vaddr32);
        }
        else
        {
            return vaddr32 & 0x1fffffff;
        }
    }
    else
    {
        // KUSEG
        return _lookupTLB(state, vaddr32);
    }
}
