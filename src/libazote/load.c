#include <stdio.h>
#include <libazote/libazote.h>

void azLoadFile(AzState* state, const char* path)
{
    FILE* file = fopen(path, "r");
    fseek(file, 0, SEEK_END);
    uint64_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    free(state->cart);
    state->cart = malloc(size);
    state->cartSize = size;
    fread(state->cart, size, 1, file);
    fclose(file);

    state->cpu.registers[20] = 0x1;
    state->cpu.registers[22] = 0x3F;
    state->cpu.registers[29] = 0xa4001ff0;
    state->cpu.pc = 0xa4000040;
    state->cpu.pc2 = state->cpu.pc + 4;

    memcpy(state->spDmem, state->cart, 0x1000);
    azDebugDumpState(state);
}
