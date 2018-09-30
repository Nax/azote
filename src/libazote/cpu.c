#include <stdio.h>
#include <libazote/libazote.h>

#define _       AZOTE_INST_NONE
#define I       AZOTE_INST_I
#define J       AZOTE_INST_J

static uint8_t const kInstructionTableCommonType[64] = {
    _, _, J, J, I, I, I, I,
    I, I, I, I, I, I, I, I,
    _, _, _, _, I, I, I, I,
    I, I, I, I, _, _, _, _,
    I, I, I, I, I, I, I, I,
    I, I, _, I, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
};

#undef _
#undef I
#undef J

#define _(x)   (azOp ## x)

static void* kInstructionTableCommon[64] = {
    NULL, NULL, _(J), _(JAL), _(BEQ), _(BNE), _(BLEZ), _(BGTZ),
    _(ADDI), _(ADDIU), _(SLTI), _(SLTIU), _(ANDI), _(ORI), _(XORI), _(LUI),
    NULL, NULL, NULL, NULL, _(BEQL), _(BNEL), _(BLEZL), _(BGTZL),
    _(DADDI), _(DADDIU), _(LDL), _(LDR), NULL, NULL, NULL, NULL,
    _(LB), _(LH), _(LWL), _(LW), _(LBU), _(LHU), _(LWR), _(LWU),
    _(SB), _(SH), NULL, _(SW), NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static AzProcInstructionR* const kInstructionTableSpecial[64] = {
    _(SLL), NULL, _(SRL), _(SRA), _(SLLV), NULL, _(SRLV), _(SRAV),
    _(JR), _(JALR), NULL, NULL, NULL, NULL, NULL, NULL,
    _(MFHI), _(MTHI), _(MFLO), _(MTLO), _(DSLLV), NULL, _(DSRLV), _(DSRAV),
    _(MULT), _(MULTU), _(DIV), _(DIVU), _(DMULT), _(DMULTU), _(DDIV), _(DDIVU),
    _(ADD), _(ADDU), _(SUB), _(SUBU), _(AND), _(OR), _(XOR), _(NOR),
    NULL, NULL, _(SLT), _(SLTU), _(DADD), _(DADDU), _(DSUB), _(DSUBU),
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    _(DSLL), NULL, _(DSRL), _(DSRA), _(DSLL32), NULL, _(DSRL32), _(DSRA32),
};

static AzProcInstructionCOP0* const kInstructionTableCOP0[32] = {
    _(MFC0), NULL, NULL, NULL,
    _(MTC0), NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
};

#undef _

static void _execInstructionCop0(AzState* state, uint32_t opcode)
{
    uint8_t selector = (opcode >> 21) & 0x1f;
    uint8_t rt = (opcode >> 16) & 0x1f;
    uint8_t rd = (opcode >> 11) & 0x1f;

    AzProcInstructionCOP0* instr = kInstructionTableCOP0[selector];
    printf("COP0\n");
    instr(state, rt, rd);
}

static void _execInstructionI(AzState* state, uint32_t opcode)
{
    uint8_t selector = (opcode >> 26);
    uint8_t rs = (opcode >> 21) & 0x1f;
    uint8_t rt = (opcode >> 16) & 0x1f;
    uint16_t imm = opcode & 0xffff;

    AzProcInstructionI* instr = (AzProcInstructionI*)kInstructionTableCommon[selector];
    printf("I\n");
    instr(state, rs, rt, imm);
}

static void _execInstructionJ(AzState* state, uint32_t opcode)
{
    uint8_t selector = (opcode >> 26);
    uint32_t target = opcode & 0x3ffffff;

    AzProcInstructionJ* instr = (AzProcInstructionJ*)kInstructionTableCommon[selector];
    printf("J\n");
    instr(state, target);
}

static void _execInstructionR(AzState* state, uint32_t opcode)
{
    uint8_t selector = (opcode & 0x3f);
    uint8_t sa = (opcode >> 6) & 0x1f;
    uint8_t rd = (opcode >> 11) & 0x1f;
    uint8_t rt = (opcode >> 16) & 0x1f;
    uint8_t rs = (opcode >> 21) & 0x1f;

    AzProcInstructionR* instr = kInstructionTableSpecial[selector];
    printf("R\n");
    instr(state, rs, rt, rd, sa);
}

static void _execInstructionCommon(AzState* state, uint32_t opcode)
{
    uint8_t selector = (opcode >> 26);
    uint8_t type = kInstructionTableCommonType[selector];

    switch (type)
    {
    case AZOTE_INST_I:
        _execInstructionI(state, opcode);
        break;
    case AZOTE_INST_J:
        _execInstructionJ(state, opcode);
        break;
    default:
        abort();
    }
}

static void _execInstruction(AzState* state, uint32_t opcode)
{
    uint8_t op = (opcode >> 26);

    switch (op)
    {
    case 0x00:
        _execInstructionR(state, opcode);
        return;
    case 0x10:
        _execInstructionCop0(state, opcode);
        return;
    default:
        _execInstructionCommon(state, opcode);
        return;
    }
}

void azRun(AzState* state)
{
    int debug = 0;
    for (;;)
    {
        /*if ((state->cpu.pc & 0xffffffff) == 0xA400090C)
            debug = 1; */

        uint32_t opcode = azMemoryRead32(state, state->cpu.pc);
        printf("PC:  0x%016llx   Op: 0x%08x\n", state->cpu.pc, opcode);
        printf("PC2: 0x%016llx\n", state->cpu.pc2);
        if (debug)
        {
            azDebugDumpState(state);
            getchar();
        }
        state->cpu.pc = state->cpu.pc2;
        state->cpu.pc2 += 4;
        _execInstruction(state, opcode);
    }
}
