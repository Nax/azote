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

    state->cpu.registers[0] = 0;
    state->cpu.registers[1] = 0x1;
    state->cpu.registers[2] = 0x0ebda536;
    state->cpu.registers[3] = 0x0ebda536;
    state->cpu.registers[4] = 0x0000a536;
    state->cpu.registers[5] = 0xffffffffc95973d5;
    state->cpu.registers[6] = 0xffffffffa4001f0c;
    state->cpu.registers[7] = 0xFFFFFFFFA4001F08LL;
	state->cpu.registers[8] = 0x00000000000000C0LL;
	state->cpu.registers[9] = 0x0000000000000000LL;
	state->cpu.registers[10] = 0x0000000000000040LL;
	state->cpu.registers[11] = 0xFFFFFFFFA4000040LL;
    state->cpu.registers[12] = 0xffffffffed10d0b3;
    state->cpu.registers[13] = 0x000000001402a4cc;
    state->cpu.registers[14] = 0x2449a366;
    state->cpu.registers[15] = 0x3103e121;
    state->cpu.registers[16] = 0x0000000000000000LL;
	state->cpu.registers[17] = 0x0000000000000000LL;
	state->cpu.registers[18] = 0x0000000000000000LL;
	state->cpu.registers[19] = 0x0000000000000000LL;
    state->cpu.registers[20] = 0x1;
    state->cpu.registers[21] = 0x0;
    state->cpu.registers[22] = 0x3f;
    state->cpu.registers[23] = 0x0;
    state->cpu.registers[24] = 0x3;
    state->cpu.registers[25] = 0xffffffff9debb54f;
    state->cpu.registers[29] = 0xa4001ff0;
    state->cpu.registers[31] = 0xffffffffa4001550;

    state->cpu.pc = 0xffffffffa4000040;
    state->cpu.pc2 = state->cpu.pc + 4;

    state->cop0.registers[4] = 0x007ffff0;
    state->cop0.registers[8] = 0xffffffff;
    state->cop0.registers[9] = 0x5000;
    state->cop0.registers[12] = 0x34000000;
    state->cop0.registers[13] = 0x5c;
    state->cop0.registers[14] = 0xffffffff;
    state->cop0.registers[16] = 0x0006e463;
    state->cop0.registers[30] = 0xffffffff;

    state->piRegisters[5] = 0x80;
    state->piRegisters[6] = 0x37;
    state->piRegisters[7] = 0x12;
    state->piRegisters[8] = 0x40;

    memcpy(state->spDmem, state->cart, 0x1000);
    azDebugDumpState(state);
}
