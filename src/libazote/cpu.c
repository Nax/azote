#include <stdio.h>
#include <libazote/libazote.h>

void azRun(AzState* state)
{
    for (;;)
    {
        uint32_t opcode = azMemoryRead32(state, state->cpu.pc);
        printf("PC: 0x%016llx   Op: 0x%08x\n", state->cpu.pc, opcode);
        state->cpu.pc += 4;
    }
}
