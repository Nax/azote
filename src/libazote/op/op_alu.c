#include <libazote/libazote.h>

AZOTE_PROTO_R(azOpSLL)
{
    if (!rd) return;

    uint32_t value = (uint32_t)state->cpu.registers[rt] << sa;
    state->cpu.registers[rd] = (int32_t)value;
}

AZOTE_PROTO_R(azOpSRL)
{
    if (!rd) return;

    uint32_t value = (uint32_t)state->cpu.registers[rt] >> sa;
    state->cpu.registers[rd] = (int32_t)value;
}
