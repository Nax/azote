#include <stdio.h>
#include <libazote/libazote.h>

void azLoadFile(AzState* state, const char* path)
{
    int cic;

    FILE* file = fopen(path, "r");
    fseek(file, 0, SEEK_END);
    uint64_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    free(state->cart);
    state->cart = malloc(size);
    state->cartSize = size;
    fread(state->cart, size, 1, file);
    fclose(file);

    cic = azRomGetCIC(state->cart);

    if (cic == CIC_UNKNOWN)
    {
        printf("Unknown CIC, defaulting to 6102\n");
        cic = CIC_6102;
    }

    state->cpu.registers[ 0] = 0;
    state->cpu.registers[ 1] = 0x1;
    state->cpu.registers[ 2] = 0x0ebda536;
    state->cpu.registers[ 3] = 0x0ebda536;
    state->cpu.registers[ 4] = 0x0000a536;
    state->cpu.registers[ 6] = 0xffffffffa4001f0c;
    state->cpu.registers[ 7] = 0xffffffffa4001f08;
	state->cpu.registers[ 8] = 0x00000000000000C0;
	state->cpu.registers[ 9] = 0x0000000000000000;
	state->cpu.registers[10] = 0x0000000000000040;
	state->cpu.registers[11] = 0xffffffffa4000040;
    state->cpu.registers[12] = 0xffffffffed10d0b3;
    state->cpu.registers[13] = 0x000000001402a4cc;
    state->cpu.registers[15] = 0x3103e121;
    state->cpu.registers[16] = 0x0000000000000000;
	state->cpu.registers[17] = 0x0000000000000000;
	state->cpu.registers[18] = 0x0000000000000000;
	state->cpu.registers[19] = 0x0000000000000000;
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

    state->cop0.registers[COP0_REG_CONTEXT] = 0x007ffff0;
    state->cop0.registers[COP0_REG_BADVADDR] = 0xffffffff;
    state->cop0.registers[COP0_REG_COUNT] = 0x5000;
    state->cop0.registers[COP0_REG_STATUS] = 0x34000000;
    state->cop0.registers[COP0_REG_CAUSE] = 0x5c;
    state->cop0.registers[COP0_REG_EPC] = 0xffffffff;
    state->cop0.registers[COP0_REG_CONFIG] = 0x0006e463;
    state->cop0.registers[COP0_REG_ERROR_EPC] = 0xffffffff;

    state->piRegisters[5] = 0x80;
    state->piRegisters[6] = 0x37;
    state->piRegisters[7] = 0x12;
    state->piRegisters[8] = 0x40;

    state->rsp.cregs[RSP_CREG_DMA_CACHE] = 0x00;
    state->rsp.cregs[RSP_CREG_DMA_DRAM] = 0x00;
    state->rsp.cregs[RSP_CREG_DMA_READ_LENGTH] = 0x00;
    state->rsp.cregs[RSP_CREG_DMA_WRITE_LENGTH] = 0x00;
    state->rsp.cregs[RSP_CREG_SP_STATUS] = 0x01;
    state->rsp.cregs[RSP_CREG_CMD_STATUS] = 0x98;

    memcpy(state->spDmem, state->cart, 0x1000);

    switch (cic)
    {
    case CIC_6102:
        state->cpu.registers[ 1] = 0x0000000000000001;
		state->cpu.registers[ 2] = 0x000000000ebda536;
		state->cpu.registers[ 3] = 0x000000000ebda536;
		state->cpu.registers[ 4] = 0x000000000000a536;
        state->cpu.registers[ 5] = 0xffffffffc95973d5;
		state->cpu.registers[12] = 0xffffffffed10d0b3;
		state->cpu.registers[13] = 0x000000001402a4cc;
        state->cpu.registers[14] = 0x000000002449a366;
		state->cpu.registers[15] = 0x000000003103e121;
		state->cpu.registers[22] = 0x000000000000003f;
		state->cpu.registers[25] = 0xffffffff9debb54f;
        break;
    case CIC_6103:
        state->cpu.registers[ 1] = 0x0000000000000001;
        state->cpu.registers[ 2] = 0x0000000049a5ee96;
        state->cpu.registers[ 3] = 0x0000000049a5ee96;
        state->cpu.registers[ 4] = 0x000000000000ee96;
        state->cpu.registers[ 5] = 0xffffffff95315a28;
        state->cpu.registers[12] = 0xffffffffce9dfbf7;
        state->cpu.registers[13] = 0xffffffffce9dfbf7;
        state->cpu.registers[14] = 0x000000005baca1df;
        state->cpu.registers[15] = 0x0000000018b63d28;
        state->cpu.registers[22] = 0x0000000000000078;
        state->cpu.registers[25] = 0xffffffff825b21c9;
        break;
    case CIC_6105:
        *(uint32_t*)(state->spImem + 0x00) = bswap32(0x3c0dbfc0);
        *(uint32_t*)(state->spImem + 0x04) = bswap32(0x8da807fc);
        *(uint32_t*)(state->spImem + 0x08) = bswap32(0x25ad07c0);
		*(uint32_t*)(state->spImem + 0x0c) = bswap32(0x31080080);
		*(uint32_t*)(state->spImem + 0x10) = bswap32(0x5500fffc);
		*(uint32_t*)(state->spImem + 0x14) = bswap32(0x3c0dbfc0);
		*(uint32_t*)(state->spImem + 0x18) = bswap32(0x8da80024);
		*(uint32_t*)(state->spImem + 0x1c) = bswap32(0x3c0bb000);

        state->cpu.registers[ 1] = 0x0000000000000000;
		state->cpu.registers[ 2] = 0xfffffffff58b0fbf;
		state->cpu.registers[ 3] = 0xfffffffff58b0fbf;
		state->cpu.registers[ 4] = 0x0000000000000fbf;
        state->cpu.registers[ 5] = 0x000000005493fb9a;
		state->cpu.registers[12] = 0xffffffff9651f81e;
		state->cpu.registers[13] = 0x000000002d42aac5;
        state->cpu.registers[14] = 0xffffffffc2c20384;
		state->cpu.registers[15] = 0x0000000056584d60;
		state->cpu.registers[22] = 0x0000000000000091;
		state->cpu.registers[25] = 0xffffffffcdce565f;
        break;
    }
    azDebugDumpState(state);
}
