#include <libazote/libazote.h>

AZOTE_PROTO_COP(azOpMTC0)
{
    if (rd == 12)
    {
        //printf("STATUS: 0x%016llx    PC: 0x%016llx\n", state->cpu.registers[rt]);
        // TODO: Catch this
    }
    state->cop0.registers[rd] = state->cpu.registers[rt];
}

AZOTE_PROTO_COP(azOpMFC0)
{
    if (rt)
    {
        state->cpu.registers[rt] = state->cop0.registers[rd];
    }
}

AZOTE_PROTO_COPCO(azOpTLBR)
{

}

AZOTE_PROTO_COPCO(azOpTLBWI)
{

}

AZOTE_PROTO_COPCO(azOpTLBWR)
{

}

AZOTE_PROTO_COPCO(azOpTLBP)
{

}

AZOTE_PROTO_COPCO(azOpERET)
{
    printf("ERET, Status: 0x%016llx EPC: 0x%016llx\n", state->cop0.registers[12], state->cop0.registers[14]);
    state->verbose = 1;
    getchar();
    if (state->cop0.registers[12] & (1 << 2))
    {
        state->cop0.registers[12] &= 0xfffffffffffffffb;
        state->cpu.pc = state->cop0.registers[30];
    }
    else
    {
        state->cop0.registers[12] &= 0xfffffffffffffffd;
        state->cpu.pc = state->cop0.registers[14];
    }
    state->cpu.pc2 = state->cpu.pc + 4;
    state->cpu.ll = 0;
}
