#include <stdio.h>
#include <string.h>
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

#define OP_SPECIAL  0x00
#define OP_REGIMM   0x01
#define OP_J        0x02
#define OP_JAL      0x03
#define OP_BEQ      0x04
#define OP_BNE      0x05
#define OP_BLEZ     0x06
#define OP_BGTZ     0x07
#define OP_ADDI     0x08
#define OP_ADDIU    0x09
#define OP_SLTI     0x0a
#define OP_SLTIU    0x0b
#define OP_ANDI     0x0c
#define OP_ORI      0x0d
#define OP_XORI     0x0e
#define OP_LUI      0x0f
#define OP_COP0     0x10
#define OP_COP1     0x11
#define OP_COP2     0x12
#define OP_BEQL     0x14
#define OP_BNEL     0x15
#define OP_BLEZL    0x16
#define OP_BGTZL    0x17
#define OP_DADDI    0x18
#define OP_DADDIU   0x19
#define OP_LDL      0x1a
#define OP_LDR      0x1b
#define OP_LB       0x20
#define OP_LH       0x21
#define OP_LWL      0x22
#define OP_LW       0x23
#define OP_LBU      0x24
#define OP_LHU      0x25
#define OP_LWR      0x26
#define OP_LWU      0x27
#define OP_SB       0x28
#define OP_SH       0x29
#define OP_SWL      0x2a
#define OP_SW       0x2b
#define OP_SDL      0x2c
#define OP_SDR      0x2d
#define OP_SWR      0x2e
#define OP_CACHE    0x2f
#define OP_LL       0x30
#define OP_LWC1     0x31
#define OP_LLD      0x34
#define OP_LDC1     0x35
#define OP_LD       0x37
#define OP_SC       0x38
#define OP_SWC1     0x39
#define OP_SCD      0x3c
#define OP_SDC1     0x3d
#define OP_SD       0x3f


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

static void* const kInstructionTableFp_S[64] = {
    _(ADD_S), _(SUB_S), _(MUL_S), _(DIV_S), _(SQRT_S), _(ABS_S), _(MOV_S), _(NEG_S),
    _(ROUND_L_S), _(TRUNC_L_S), _(CEIL_L_S), _(FLOOR_L_S), _(ROUND_W_S), _(TRUNC_W_S), _(CEIL_W_S), _(FLOOR_W_S),
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, _(CVT_D_S), NULL, NULL, _(CVT_W_S), _(CVT_L_S), NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static void* const kInstructionTableFp_D[64] = {
    _(ADD_D), _(SUB_D), _(MUL_D), _(DIV_D), _(SQRT_D), _(ABS_D), _(MOV_D), _(NEG_D),
    _(ROUND_L_D), _(TRUNC_L_D), _(CEIL_L_D), _(FLOOR_L_D), _(ROUND_W_D), _(TRUNC_W_D), _(CEIL_W_D), _(FLOOR_W_D),
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    _(CVT_S_D), NULL, NULL, NULL, _(CVT_W_D), _(CVT_L_D), NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static void* const kInstructionTableFp_W[64] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    _(CVT_S_W), _(CVT_D_W), NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static void* const kInstructionTableFp_L[64] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    _(CVT_S_L), _(CVT_D_L), NULL, NULL, NULL, NULL,
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

static void _execInstructionFp_S(AzState* state, uint32_t opcode)
{
    uint8_t selector = opcode & 0x3f;
    uint8_t ft = (opcode >> 16) & 0x1f;
    uint8_t fs = (opcode >> 11) & 0x1f;
    uint8_t fd = (opcode >>  6) & 0x1f;

    AzProcInstructionFpR* instr = (AzProcInstructionFpR*)kInstructionTableFp_S[selector];
    instr(state, ft, fs, fd);
}

static void _execInstructionFp_D(AzState* state, uint32_t opcode)
{
    uint8_t selector = opcode & 0x3f;
    uint8_t ft = (opcode >> 16) & 0x1f;
    uint8_t fs = (opcode >> 11) & 0x1f;
    uint8_t fd = (opcode >>  6) & 0x1f;

    AzProcInstructionFpR* instr = (AzProcInstructionFpR*)kInstructionTableFp_D[selector];
    instr(state, ft, fs, fd);
}

static void _execInstructionFp_W(AzState* state, uint32_t opcode)
{
    uint8_t selector = opcode & 0x3f;
    uint8_t ft = (opcode >> 16) & 0x1f;
    uint8_t fs = (opcode >> 11) & 0x1f;
    uint8_t fd = (opcode >>  6) & 0x1f;

    AzProcInstructionFpR* instr = (AzProcInstructionFpR*)kInstructionTableFp_W[selector];
    instr(state, ft, fs, fd);
}

static void _execInstructionFp_L(AzState* state, uint32_t opcode)
{
    uint8_t selector = opcode & 0x3f;
    uint8_t ft = (opcode >> 16) & 0x1f;
    uint8_t fs = (opcode >> 11) & 0x1f;
    uint8_t fd = (opcode >>  6) & 0x1f;

    AzProcInstructionFpR* instr = (AzProcInstructionFpR*)kInstructionTableFp_L[selector];
    instr(state, ft, fs, fd);
}

static void _execInstructionCop1(AzState* state, uint32_t opcode)
{
    uint8_t selector = (opcode >> 21) & 0x1f;

    switch (selector)
    {
    case 0x10:
        _execInstructionFp_S(state, opcode);
        return;
    case 0x11:
        _execInstructionFp_D(state, opcode);
        return;
    case 0x14:
        _execInstructionFp_W(state, opcode);
        return;
    case 0x15:
        _execInstructionFp_L(state, opcode);
        return;
    default:
        break;
    }

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
        int bd = (state->cpu.pc2 != state->cpu.pc + 4);
        state->verbose = 1;
        state->cop0.registers[12] |= 0x02;
        state->cop0.registers[14] = state->cpu.pc;
        if (bd)
            state->cop0.registers[14] -= 4;
        state->cpu.pc = 0xffffffff80000180;
        state->cpu.pc2 = state->cpu.pc + 4;
    }
}

#define RS      ((op >> 21) & 0x1f)
#define RT      ((op >> 16) & 0x1f)
#define IMM     (op & 0xffff)
#define SIMM    ((int16_t)(op & 0xffff))

void _runCycles(AzState* state, uint32_t cycles)
{
    uint32_t op;
    uint64_t pc;
    uint64_t pc2;
    AzReg regs[32];

    pc = state->cpu.pc;
    pc2 = state->cpu.pc2;
    memcpy(regs, state->cpu.registers, 32 * sizeof(AzReg));

    for (uint32_t i = 0; i < cycles; ++i)
    {
        _handleInterrupts(state);
        op = azMemoryRead32(state, pc);
        pc = pc2;
        pc2 += 4;

        switch (op >> 26)
        {
        case OP_SPECIAL:
            break;
        case OP_REGIMM:
            break;
        case OP_J:
            pc2 = (pc & 0xfffffffff0000000) | ((op & 0x3ffffff) << 2);
            break;
        case OP_JAL:
            regs[31].u64 = pc2;
            pc2 = (pc & 0xfffffffff0000000) | ((op & 0x3ffffff) << 2);
            break;
        case OP_BEQ:
            if (regs[RS].u64 == regs[RT].u64)
                pc2 = pc + (SIMM << 2);
            break;
        case OP_BNE:
            if (regs[RS].u64 != regs[RT].u64)
                pc2 = pc + (SIMM << 2);
            break;
        case OP_BLEZ:
            if (regs[RS].u64 <= 0)
                pc2 = pc + (SIMM << 2);
            break;
        case OP_BGTZ:
            if (regs[RS].u64 > 0)
                pc2 = pc + (SIMM << 2);
            break;
        case OP_ADDI:
            if (RT)
                regs[RT].i64 = regs[RS].i32 + SIMM;
            break;
        case OP_ADDIU:
            if (RT)
                regs[RT].i64 = regs[RS].i32 + SIMM;
            break;
        case OP_SLTI:
            if (RT)
            {
                if (regs[RS].i64 < SIMM)
                    regs[RT].u64 = 1;
                else
                    regs[RT].u64 = 0;
            }
            break;
        case OP_SLTIU:
            if (RT)
            {
                if (regs[RS].u64 < IMM)
                    regs[RT].u64 = 1;
                else
                    regs[RT].u64 = 0;
            }
            break;
        case OP_ANDI:
            if (RT)
                regs[RT].u64 = regs[RS].u64 & IMM;
            break;
        case OP_ORI:
            if (RT)
                regs[RT].u64 = regs[RS].u64 | IMM;
            break;
        case OP_XORI:
            if (RT)
                regs[RT].u64 = regs[RS].u64 ^ IMM;
            break;
        case OP_LUI:
            if (RT)
                regs[RT].i64 = (SIMM << 16);
            break;
        case OP_COP0:
            break;
        case OP_COP1:
            break;
        case OP_COP2:
            break;
        case OP_BEQL:
            if (regs[RS].u64 == regs[RT].u64)
            {
                pc2 = pc + (SIMM << 2);
            }
            else
            {
                pc2 += 4;
                pc += 4;
            }
            break;
        case OP_BNEL:
            if (regs[RS].u64 != regs[RT].u64)
            {
                pc2 = pc + (SIMM << 2);
            }
            else
            {
                pc2 += 4;
                pc += 4;
            }
            break;
        case OP_BLEZL:
            if (regs[RS].u64 <= 0)
            {
                pc2 = pc + (SIMM << 2);
            }
            else
            {
                pc2 += 4;
                pc += 4;
            }
            break;
        case OP_BGTZL:
            if (regs[RS].u64 > 0)
            {
                pc2 = pc + (SIMM << 2);
            }
            else
            {
                pc2 += 4;
                pc += 4;
            }
            break;
        case OP_DADDI:
            if (RT)
                regs[RT].i64 = regs[RS].i64 + SIMM;
            break;
        case OP_DADDIU:
            if (RT)
                regs[RT].i64 = regs[RS].i64 + SIMM;
            break;
        case OP_LDL:
            break;
        case OP_LDR:
            break;
        case OP_LB:
            if (RT)
                regs[RT].i64 = (int8_t)azMemoryRead8(state, regs[RS].u64 + SIMM);
            break;
        case OP_LH:
            if (RT)
                regs[RT].i64 = (int16_t)azMemoryRead16(state, regs[RS].u64 + SIMM);
            break;
        case OP_LWL:
            break;
        case OP_LW:
            if (RT)
                regs[RT].i64 = (int32_t)azMemoryRead32(state, regs[RS].u64 + SIMM);
            break;
        case OP_LBU:
            if (RT)
                regs[RT].u64 = azMemoryRead8(state, regs[RS].u64 + SIMM);
            break;
        case OP_LHU:
            if (RT)
                regs[RT].u64 = azMemoryRead16(state, regs[RS].u64 + SIMM);
            break;
        case OP_LWR:
            break;
        case OP_LWU:
            if (RT)
                regs[RT].u64 = azMemoryRead32(state, regs[RS].u64 + SIMM);
            break;
        case OP_SB:
            azMemoryWrite8(state, regs[RS].u64 + SIMM, regs[RT].u64);
            break;
        case OP_SH:
            azMemoryWrite16(state, regs[RS].u64 + SIMM, regs[RT].u64);
            break;
        case OP_SWL:
            break;
        case OP_SW:
            azMemoryWrite32(state, regs[RS].u64 + SIMM, regs[RT].u64);
            break;
        case OP_SDL:
            break;
        case OP_SDR:
            break;
        case OP_SWR:
            break;
        case OP_CACHE:
            break;
        case OP_LL:
            break;
        case OP_LWC1:
            break;
        case OP_LLD:
            break;
        case OP_LDC1:
            break;
        case OP_LD:
            break;
        case OP_SC:
            break;
        case OP_SWC1:
            break;
        case OP_SCD:
            break;
        case OP_SDC1:
            break;
        case OP_SD:
            break;
        }
    }

    state->cpu.pc = pc;
    state->cpu.pc2 = pc2;
    memcpy(state->cpu.registers, regs, 32 * sizeof(AzReg));
}

uint64_t _getTimeNano()
{
    return clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
}

void azRun(AzState* state)
{
    static const uint32_t granularity = 256;
    static const uint64_t kPeriod = 16666666;
    uint64_t now;
    uint64_t referenceTime;
    uint64_t baseTime;
    uint64_t dt;
    uint64_t cycles;

    cycles = 0;
    referenceTime = _getTimeNano();
    baseTime = _getTimeNano();
    for (;;)
    {
        _runCycles(state, granularity);
        cycles += granularity;
        now = _getTimeNano();
        dt = now - referenceTime;
        if (dt >= kPeriod)
        {
            referenceTime += kPeriod;
            //azRcpRaiseInterrupt(state, RCP_INTR_VI);
        }
        if ((now - baseTime) >= 1000000000)
        {
            double instrPerSecond = (double)cycles / ((double)(now - baseTime) * 1e-9);
            printf("Vi/s: %.2fM   (PC: 0x%016llx)\n", instrPerSecond / 1000000, state->cpu.pc);
            baseTime = now;
            cycles = 0;
        }
    }
}
