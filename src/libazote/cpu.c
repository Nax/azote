#include <stdio.h>
#include <time.h>
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
    I, I, I, I, I, I, I, I,
    I, I, _, _, I, I, _, I,
    I, I, _, _, I, I, _, I,
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
    _(SB), _(SH), _(SWL), _(SW), _(SDL), _(SDR), _(SWR), _(CACHE),
    _(LL), _(LWC1), NULL, NULL, _(LLD), _(LDC1), NULL, _(LD),
    _(SC), _(SWC1), NULL, NULL, _(SCD), _(SDC1), NULL, _(SD),
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

static AzProcInstructionRegImm* const kInstructionTableRegImm[32] = {
    _(BLTZ), _(BGEZ), _(BLTZL), _(BGEZL), NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    _(BLTZAL), _(BGEZAL), _(BLTZALL), _(BGEZALL), NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static AzProcInstructionCop* const kInstructionTableCOP0[16] = {
    _(MFC0), NULL, NULL, NULL, _(MTC0), NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static AzProcInstructionCopCo* const kInstructionTableCOPCO0[64] = {
    NULL, _(TLBR), _(TLBWI), NULL, NULL, NULL, _(TLBWR), NULL,
    _(TLBP), NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    _(ERET), NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static AzProcInstructionCop* const kInstructionTableCOP1[32] = {
    _(MFC1), _(DMFC1), _(CFC1), NULL, _(MTC1), _(DMTC1), _(CTC1), NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

#undef _

static void _execInstructionCop0(AzState* state, uint32_t opcode)
{
    uint8_t selector = (opcode >> 21) & 0x1f;

    if (!(selector & 0x10))
    {
        uint8_t rt = (opcode >> 16) & 0x1f;
        uint8_t rd = (opcode >> 11) & 0x1f;

        AzProcInstructionCop* instr = kInstructionTableCOP0[selector];
        instr(state, rt, rd);
    }
    else
    {
        selector = opcode & 0x1f;

        AzProcInstructionCopCo* instr = kInstructionTableCOPCO0[selector];
        instr(state);
    }
}

static void _execInstructionCop1(AzState* state, uint32_t opcode)
{
    uint8_t selector = (opcode >> 21) & 0x1f;
    uint8_t rt = (opcode >> 16) & 0x1f;
    uint8_t rd = (opcode >> 11) & 0x1f;

    AzProcInstructionCop* instr = kInstructionTableCOP1[selector];
    instr(state, rt, rd);
}

static void _execInstructionRegImm(AzState* state, uint32_t opcode)
{
    uint8_t rs = (opcode >> 21) & 0x1f;
    uint8_t selector = (opcode >> 16) & 0x1f;
    uint16_t imm = opcode & 0xffff;

    AzProcInstructionRegImm* instr = kInstructionTableRegImm[selector];
    instr(state, rs, imm);
}

static void _execInstructionI(AzState* state, uint32_t opcode)
{
    uint8_t selector = (opcode >> 26);
    uint8_t rs = (opcode >> 21) & 0x1f;
    uint8_t rt = (opcode >> 16) & 0x1f;
    uint16_t imm = opcode & 0xffff;

    AzProcInstructionI* instr = (AzProcInstructionI*)kInstructionTableCommon[selector];
    instr(state, rs, rt, imm);
}

static void _execInstructionJ(AzState* state, uint32_t opcode)
{
    uint8_t selector = (opcode >> 26);
    uint32_t target = opcode & 0x3ffffff;

    AzProcInstructionJ* instr = (AzProcInstructionJ*)kInstructionTableCommon[selector];
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
    case 0x01:
        _execInstructionRegImm(state, opcode);
        break;
    case 0x10:
        _execInstructionCop0(state, opcode);
        return;
    case 0x11:
        _execInstructionCop1(state, opcode);
        return;
    default:
        _execInstructionCommon(state, opcode);
        return;
    }
}

static void _handleTimer(AzState* state)
{
    state->cop0.registers[9]++;
    if (state->cop0.registers[9] > 10000000)
        state->verbose = 1;
    if (state->cop0.registers[9] == state->cop0.registers[11])
    {
        puts("*** TIMER !!! ***");
        getchar();
    }
    if ((state->cop0.registers[9] % (1 << 20)) == 0)
        ;//printf("Count: 0x%016llx   Compare: 0x%016llx\n", state->cop0.registers[9], state->cop0.registers[11]);
}

static void _handleInterrupts(AzState* state)
{
    uint64_t status = state->cop0.registers[12];
    uint8_t pending;

    if ((status & 0x01) == 0 || (status & 0x06) != 0)
        return;
    pending = (state->cop0.registers[13] >> 8) & 0xff;
    pending &= ((status >> 8) & 0xff);
    if (pending)
    {
        puts("*** INTERRUPT ! ***");
        printf("IMASK:  0x%02llx\n", pending);
        printf("PC:     0x%016llx\n", state->cpu.pc);
        if (!state->verbose) getchar();
        state->verbose = 1;
        state->cop0.registers[12] |= 0x02;
        state->cop0.registers[14] = state->cpu.pc;
        state->cpu.pc = 0xffffffff80000180;
        state->cpu.pc2 = state->cpu.pc + 4;
    }
}

void _runCycles(AzState* state, uint32_t cycles)
{
    uint64_t pcBak;

    for (uint32_t i = 0; i < cycles; ++i)
    {
        uint32_t opcode = azMemoryRead32(state, state->cpu.pc);
        if (state->verbose)
            printf("PC:  0x%016llx   Op: 0x%08x\n", state->cpu.pc, opcode);
        if (state->debug && 0)
        {
            azDebugDumpState(state);
            char c = getchar();

            if (c == 'c')
                state->debug = 0;
            
            while (c != '\n')
                c = getchar();
        }
        pcBak = state->cpu.pc;
        state->cpu.pc = state->cpu.pc2;
        state->cpu.pc2 += 4;
        _execInstruction(state, opcode);
        if (state->cpu.pc == 0xffffffff)
        {
            puts("*** BAD PC2 ***");
            printf("Backup PC: 0x%016llx\n", pcBak);
            azDebugDumpState(state);
            getchar();
        }
        _handleTimer(state);
        _handleInterrupts(state);
    }
}

uint64_t _getTimeNano()
{
    return clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
}

void azRun(AzState* state)
{
    static const uint64_t kPeriod = 16666666;
    uint64_t referenceTime;
    uint64_t dt;

    referenceTime = _getTimeNano();
    for (;;)
    {
        _runCycles(state, 8192);
        dt = _getTimeNano() - referenceTime;
        if (dt >= kPeriod)
        {
            referenceTime += kPeriod;
            azRcpRaiseInterrupt(state, RCP_INTR_VI);
        }
    }
}
