#include <libazote/libazote.h>

AZOTE_PROTO_FP_R(azOpADD_S)
{
    state->cop1.registers[fd].f = state->cop1.registers[fs].f + state->cop1.registers[ft].f;
}

AZOTE_PROTO_FP_R(azOpADD_D)
{
    state->cop1.registers[fd].d = state->cop1.registers[fs].d + state->cop1.registers[ft].d;
}

AZOTE_PROTO_FP_R(azOpSUB_S)
{
    state->cop1.registers[fd].f = state->cop1.registers[fs].f - state->cop1.registers[ft].f;
}

AZOTE_PROTO_FP_R(azOpSUB_D)
{
    state->cop1.registers[fd].d = state->cop1.registers[fs].d - state->cop1.registers[ft].d;
}

AZOTE_PROTO_FP_R(azOpMUL_S)
{
    state->cop1.registers[fd].f = state->cop1.registers[fs].f * state->cop1.registers[ft].f;
}

AZOTE_PROTO_FP_R(azOpMUL_D)
{
    state->cop1.registers[fd].d = state->cop1.registers[fs].d * state->cop1.registers[ft].d;
}

AZOTE_PROTO_FP_R(azOpDIV_S)
{
    state->cop1.registers[fd].f = state->cop1.registers[fs].f / state->cop1.registers[ft].f;
}

AZOTE_PROTO_FP_R(azOpDIV_D)
{
    state->cop1.registers[fd].d = state->cop1.registers[fs].d / state->cop1.registers[ft].d;
}

AZOTE_PROTO_FP_R(azOpSQRT_S)
{
    state->cop1.registers[fd].f = sqrtf(state->cop1.registers[fs].f);
}

AZOTE_PROTO_FP_R(azOpSQRT_D)
{
    state->cop1.registers[fd].d = sqrt(state->cop1.registers[fs].d);
}

AZOTE_PROTO_FP_R(azOpABS_S)
{
    state->cop1.registers[fd].f = fabsf(state->cop1.registers[fs].f);
}

AZOTE_PROTO_FP_R(azOpABS_D)
{
    state->cop1.registers[fd].d = fabs(state->cop1.registers[fs].d);
}

AZOTE_PROTO_FP_R(azOpMOV_S)
{
    state->cop1.registers[fd].f = state->cop1.registers[fs].f;
}

AZOTE_PROTO_FP_R(azOpMOV_D)
{
    state->cop1.registers[fd].d = state->cop1.registers[fs].d;
}

AZOTE_PROTO_FP_R(azOpNEG_S)
{
    state->cop1.registers[fd].f = -state->cop1.registers[fs].f;
}

AZOTE_PROTO_FP_R(azOpNEG_D)
{
    state->cop1.registers[fd].d = -state->cop1.registers[fs].d;
}

AZOTE_PROTO_FP_R(azOpROUND_W_S)
{
    state->cop1.registers[fd].u32 = fpS2W(roundf(state->cop1.registers[fs].f));
}

AZOTE_PROTO_FP_R(azOpROUND_W_D)
{
    state->cop1.registers[fd].u32 = fpD2W(round(state->cop1.registers[fs].d));
}

AZOTE_PROTO_FP_R(azOpROUND_L_S)
{
    state->cop1.registers[fd].u64 = fpS2L(roundf(state->cop1.registers[fs].f));
}

AZOTE_PROTO_FP_R(azOpROUND_L_D)
{
    state->cop1.registers[fd].u64 = fpD2L(round(state->cop1.registers[fs].d));
}

AZOTE_PROTO_FP_R(azOpTRUNC_W_S)
{
    state->cop1.registers[fd].u32 = fpS2W(truncf(state->cop1.registers[fs].f));
}

AZOTE_PROTO_FP_R(azOpTRUNC_W_D)
{
    state->cop1.registers[fd].u32 = fpD2W(trunc(state->cop1.registers[fs].d));
}

AZOTE_PROTO_FP_R(azOpTRUNC_L_S)
{
    state->cop1.registers[fd].u64 = fpS2L(truncf(state->cop1.registers[fs].f));
}

AZOTE_PROTO_FP_R(azOpTRUNC_L_D)
{
    state->cop1.registers[fd].u64 = fpD2L(trunc(state->cop1.registers[fs].d));
}

AZOTE_PROTO_FP_R(azOpCEIL_W_S)
{
    state->cop1.registers[fd].u32 = fpS2W(ceilf(state->cop1.registers[fs].f));
}

AZOTE_PROTO_FP_R(azOpCEIL_W_D)
{
    state->cop1.registers[fd].u32 = fpD2W(ceil(state->cop1.registers[fs].d));
}

AZOTE_PROTO_FP_R(azOpCEIL_L_S)
{
    state->cop1.registers[fd].u64 = fpS2L(ceilf(state->cop1.registers[fs].f));
}

AZOTE_PROTO_FP_R(azOpCEIL_L_D)
{
    state->cop1.registers[fd].u64 = fpD2L(ceil(state->cop1.registers[fs].d));
}

AZOTE_PROTO_FP_R(azOpFLOOR_W_S)
{
    state->cop1.registers[fd].u32 = fpS2W(floorf(state->cop1.registers[fs].f));
}

AZOTE_PROTO_FP_R(azOpFLOOR_W_D)
{
    state->cop1.registers[fd].u32 = fpD2W(floor(state->cop1.registers[fs].d));
}

AZOTE_PROTO_FP_R(azOpFLOOR_L_S)
{
    state->cop1.registers[fd].u64 = fpS2L(floorf(state->cop1.registers[fs].f));
}

AZOTE_PROTO_FP_R(azOpFLOOR_L_D)
{
    state->cop1.registers[fd].u64 = fpD2L(floor(state->cop1.registers[fs].d));
}
