#include <stdio.h>
#include <libazote/libazote.h>

uint32_t azReadPIF(AzState* state, uint32_t addr)
{
    printf("PIF Read: 0x%08x\n", addr);
    return 0;
}

void azWritePIF(AzState* state, uint32_t addr, uint32_t value)
{
    printf("PIF Write:0x%08x (0x%08x)\n", addr, value);
}
