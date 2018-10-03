#include <libazote/libazote.h>

AZOTE_PROTO_FP_R(azOpCVT_S_D)
{
    state->cop1.registers[fd].f = (float)state->cop1.registers[fs].d;
}

AZOTE_PROTO_FP_R(azOpCVT_S_W)
{
    state->cop1.registers[fd].f = fpW2S(state->cop1.registers[fs].u32);
}

AZOTE_PROTO_FP_R(azOpCVT_S_L)
{
    state->cop1.registers[fd].f = fpL2S(state->cop1.registers[fs].u64);
}

AZOTE_PROTO_FP_R(azOpCVT_D_S)
{
    state->cop1.registers[fd].d = (double)state->cop1.registers[fs].f;
}

AZOTE_PROTO_FP_R(azOpCVT_D_W)
{
    state->cop1.registers[fd].d = fpW2D(state->cop1.registers[fs].u32);
}

AZOTE_PROTO_FP_R(azOpCVT_D_L)
{
    state->cop1.registers[fd].d = fpL2D(state->cop1.registers[fs].u64);
}

AZOTE_PROTO_FP_R(azOpCVT_W_S)
{
    state->cop1.registers[fd].u32 = fpS2W(state->cop1.registers[fs].f);
}

AZOTE_PROTO_FP_R(azOpCVT_W_D)
{
    state->cop1.registers[fd].u32 = fpD2W(state->cop1.registers[fs].d);
}

AZOTE_PROTO_FP_R(azOpCVT_L_S)
{
    state->cop1.registers[fd].u64 = fpS2L(state->cop1.registers[fs].f);
}

AZOTE_PROTO_FP_R(azOpCVT_L_D)
{
    state->cop1.registers[fd].u64 = fpD2L(state->cop1.registers[fs].d);
}
