#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <libazote/libazote.h>

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

#define OP_SPECIAL_SLL      0x00
#define OP_SPECIAL_SRL      0x02
#define OP_SPECIAL_SRA      0x03
#define OP_SPECIAL_SLLV     0x04
#define OP_SPECIAL_SRLV     0x06
#define OP_SPECIAL_SRAV     0x07
#define OP_SPECIAL_JR       0x08
#define OP_SPECIAL_JALR     0x09
#define OP_SPECIAL_SYSCALL  0x0c
#define OP_SPECIAL_BREAK    0x0d
#define OP_SPECIAL_SYNC     0x0f
#define OP_SPECIAL_MFHI     0x10
#define OP_SPECIAL_MTHI     0x11
#define OP_SPECIAL_MFLO     0x12
#define OP_SPECIAL_MTLO     0x13
#define OP_SPECIAL_DSLLV    0x14
#define OP_SPECIAL_DSRLV    0x16
#define OP_SPECIAL_DSRAV    0x17
#define OP_SPECIAL_MULT     0x18
#define OP_SPECIAL_MULTU    0x19
#define OP_SPECIAL_DIV      0x1a
#define OP_SPECIAL_DIVU     0x1b
#define OP_SPECIAL_DMULT    0x1c
#define OP_SPECIAL_DMULTU   0x1d
#define OP_SPECIAL_DDIV     0x1e
#define OP_SPECIAL_DDIVU    0x1f
#define OP_SPECIAL_ADD      0x20
#define OP_SPECIAL_ADDU     0x21
#define OP_SPECIAL_SUB      0x22
#define OP_SPECIAL_SUBU     0x23
#define OP_SPECIAL_AND      0x24
#define OP_SPECIAL_OR       0x25
#define OP_SPECIAL_XOR      0x26
#define OP_SPECIAL_NOR      0x27
#define OP_SPECIAL_SLT      0x2a
#define OP_SPECIAL_SLTU     0x2b
#define OP_SPECIAL_DADD     0x2c
#define OP_SPECIAL_DADDU    0x2d
#define OP_SPECIAL_DSUB     0x2e
#define OP_SPECIAL_DSUBU    0x2f
#define OP_SPECIAL_TGE      0x30
#define OP_SPECIAL_TGEU     0x31
#define OP_SPECIAL_TLT      0x32
#define OP_SPECIAL_TLTU     0x33
#define OP_SPECIAL_TEQ      0x34
#define OP_SPECIAL_TNE      0x36
#define OP_SPECIAL_DSLL     0x38
#define OP_SPECIAL_DSRL     0x3a
#define OP_SPECIAL_DSRA     0x3b
#define OP_SPECIAL_DSLL32   0x3c
#define OP_SPECIAL_DSRL32   0x3e
#define OP_SPECIAL_DSRA32   0x3f

#define OP_REGIMM_BLTZ      0x00
#define OP_REGIMM_BGEZ      0x01
#define OP_REGIMM_BLTZL     0x02
#define OP_REGIMM_BGEZL     0x03
#define OP_REGIMM_TGEI      0x08
#define OP_REGIMM_TGEIU     0x09
#define OP_REGIMM_TLTI      0x0a
#define OP_REGIMM_TLTIU     0x0b
#define OP_REGIMM_TEQI      0x0c
#define OP_REGIMM_TNEI      0x0e
#define OP_REGIMM_BLTZAL    0x10
#define OP_REGIMM_BGEZAL    0x11
#define OP_REGIMM_BLTZALL   0x12
#define OP_REGIMM_BGEZALL   0x13

#define OP_COP_MF           0x00
#define OP_COP_DMF          0x01
#define OP_COP_CF           0x02
#define OP_COP_MT           0x04
#define OP_COP_DMT          0x05
#define OP_COP_CT           0x06
#define OP_COP_BC           0x08

#define OP_CP0_TLBR         0x01
#define OP_CP0_TLBWI        0x02
#define OP_CP0_TLBWR        0x06
#define OP_CP0_TLBP         0x08
#define OP_CP0_ERET         0x18

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
        printf("INTERRUPT\n");
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
#define RD      ((op >> 11) & 0x1f)
#define SA      ((op >>  6) & 0x1f)
#define IMM     ((uint64_t)(op & 0xffff))
#define SIMM    ((int64_t)((int16_t)(op & 0xffff)))

static int32_t sra32(int32_t x, uint8_t shift)
{
    return (int32_t)(((uint32_t)x >> shift) | -(((uint32_t)x & ~(UINT32_MAX >> 1)) >> shift));
}

uint64_t pcBak;

void _runCycles(AzState* state, uint32_t cycles)
{
    uint32_t op;
    uint64_t pc;
    uint64_t pc2;
    uint64_t tmp;
    AzReg* regs = (AzReg*)(&state->cpu.registers[0]);
    AzReg hi;
    AzReg lo;

    pc = state->cpu.pc;
    pc2 = state->cpu.pc2;
    //memcpy(regs, state->cpu.registers, 32 * sizeof(AzReg));
    hi.u64 = state->cpu.hi;
    lo.u64 = state->cpu.lo;

    for (uint32_t i = 0; i < cycles; ++i)
    {
        _handleInterrupts(state);
        op = azMemoryRead32(state, pc);
        pcBak = pc;
        pc = pc2;
        pc2 += 4;

        switch (op >> 26)
        {
        case OP_SPECIAL:
            switch (op & 0x3f)
            {
            case OP_SPECIAL_SLL:
                if (RD) regs[RD].i64 = (int32_t)(regs[RT].i32 << SA);
                break;
            case OP_SPECIAL_SRL:
                if (RD) regs[RD].i64 = (int32_t)(regs[RT].u32 >> SA);
                break;
            case OP_SPECIAL_SRA:
                if (RD) regs[RD].i64 = sra32(regs[RT].i32, SA);
                break;
            case OP_SPECIAL_SLLV:
                if (RD) regs[RD].i64 = (int32_t)(regs[RT].i32 << (regs[RS].u64 & 0x1f));
                break;
            case OP_SPECIAL_SRLV:
                if (RD) regs[RD].i64 = (int32_t)(regs[RT].u32 >> (regs[RS].u64 & 0x1f));
                break;
            case OP_SPECIAL_SRAV:
                if (RD) regs[RD].i64 = sra32(regs[RT].i32, regs[RS].u64 & 0x1f);
                break;
            case OP_SPECIAL_JR:
                pc2 = regs[RS].u64;
                break;
            case OP_SPECIAL_JALR:
                if (RD) regs[RD].u64 = pc2;
                pc2 = regs[RS].u64;
                break;
            case OP_SPECIAL_SYSCALL:
                break;
            case OP_SPECIAL_BREAK:
                break;
            case OP_SPECIAL_SYNC:
                break;
            case OP_SPECIAL_MFHI:
                if (RD) regs[RD].u64 = hi.u64;
                break;
            case OP_SPECIAL_MTHI:
                hi.u64 = regs[RD].u64;
                break;
            case OP_SPECIAL_MFLO:
                if (RD) regs[RD].u64 = lo.u64;
                break;
            case OP_SPECIAL_MTLO:
                lo.u64 = regs[RD].u64;
                break;
            case OP_SPECIAL_DSLLV:
                break;
            case OP_SPECIAL_DSRLV:
                break;
            case OP_SPECIAL_DSRAV:
                break;
            case OP_SPECIAL_MULT:
                tmp = (int64_t)regs[RS].i32 * (int64_t)regs[RT].i32;
                hi.i64 = (int32_t)(tmp >> 32);
                lo.i64 = (int32_t)(tmp & 0xffffffff);
                break;
            case OP_SPECIAL_MULTU:
                tmp = (uint64_t)regs[RS].u32 * (uint64_t)regs[RT].u32;
                hi.i64 = (int32_t)(tmp >> 32);
                lo.i64 = (int32_t)(tmp & 0xffffffff);
                break;
            case OP_SPECIAL_DIV:
                lo.i64 = (int32_t)(regs[RS].i32 / regs[RT].i32);
                hi.i64 = (int32_t)(regs[RS].i32 % regs[RT].i32);
                break;
            case OP_SPECIAL_DIVU:
                lo.i64 = (int32_t)(regs[RS].u32 / regs[RT].u32);
                hi.i64 = (int32_t)(regs[RS].u32 % regs[RT].u32);
                break;
            case OP_SPECIAL_DMULT:
                break;
            case OP_SPECIAL_DMULTU:
                break;
            case OP_SPECIAL_DDIV:
                break;
            case OP_SPECIAL_DDIVU:
                break;
            case OP_SPECIAL_ADD:
                if (RD) regs[RD].i64 = (int32_t)(regs[RS].u32 + regs[RT].u32);
                break;
            case OP_SPECIAL_ADDU:
                if (RD) regs[RD].i64 = (int32_t)(regs[RS].u32 + regs[RT].u32);
                break;
            case OP_SPECIAL_SUB:
                if (RD) regs[RD].i64 = (int32_t)(regs[RS].u32 - regs[RT].u32);
                break;
            case OP_SPECIAL_SUBU:
                if (RD) regs[RD].i64 = (int32_t)(regs[RS].u32 - regs[RT].u32);
                break;
            case OP_SPECIAL_AND:
                if (RD) regs[RD].u64 = regs[RS].u64 & regs[RT].u64;
                break;
            case OP_SPECIAL_OR:
                if (RD) regs[RD].u64 = regs[RS].u64 | regs[RT].u64;
                break;
            case OP_SPECIAL_XOR:
                if (RD) regs[RD].u64 = regs[RS].u64 ^ regs[RT].u64;
                break;
            case OP_SPECIAL_NOR:
                if (RD) regs[RD].u64 = ~(regs[RS].u64 | regs[RT].u64);
                break;
            case OP_SPECIAL_SLT:
                if (RD)
                {
                    if (regs[RS].i64 < regs[RT].i64)
                        regs[RD].u64 = 1;
                    else
                        regs[RD].u64 = 0;
                }
                break;
            case OP_SPECIAL_SLTU:
                if (RD)
                {
                    if (regs[RS].u64 < regs[RT].u64)
                        regs[RD].u64 = 1;
                    else
                        regs[RD].u64 = 0;
                }
                break;
            case OP_SPECIAL_DADD:
                break;
            case OP_SPECIAL_DADDU:
                break;
            case OP_SPECIAL_DSUB:
                break;
            case OP_SPECIAL_DSUBU:
                break;
            case OP_SPECIAL_TGE:
                break;
            case OP_SPECIAL_TGEU:
                break;
            case OP_SPECIAL_TLT:
                break;
            case OP_SPECIAL_TLTU:
                break;
            case OP_SPECIAL_TEQ:
                break;
            case OP_SPECIAL_TNE:
                break;
            case OP_SPECIAL_DSLL:
                break;
            case OP_SPECIAL_DSRL:
                break;
            case OP_SPECIAL_DSRA:
                break;
            case OP_SPECIAL_DSLL32:
                break;
            case OP_SPECIAL_DSRL32:
                break;
            case OP_SPECIAL_DSRA32:
                break;
            }
            break;
        case OP_REGIMM:
            switch (RT)
            {
            case OP_REGIMM_BLTZ:
                if (regs[RS].i64 < 0) pc2 = pc + (SIMM << 2);
                break;
            case OP_REGIMM_BGEZ:
                if (regs[RS].i64 >= 0) pc2 = pc + (SIMM << 2);
                break;
            case OP_REGIMM_BLTZL:
                if (regs[RS].i64 < 0)
                {
                    pc2 = pc + (SIMM << 2);
                }
                else
                {
                    pc += 4;
                    pc2 = pc + 4;
                }
                break;
            case OP_REGIMM_BGEZL:
                if (regs[RS].i64 >= 0)
                {
                    pc2 = pc + (SIMM << 2);
                }
                else
                {
                    pc += 4;
                    pc2 = pc + 4;
                }
                break;
            case OP_REGIMM_TGEI:
                break;
            case OP_REGIMM_TGEIU:
                break;
            case OP_REGIMM_TLTI:
                break;
            case OP_REGIMM_TLTIU:
                break;
            case OP_REGIMM_TEQI:
                break;
            case OP_REGIMM_TNEI:
                break;
            case OP_REGIMM_BLTZAL:
                /* Debug */
                azDebugDumpState(state);
                getchar();
                regs[31].u64 = pc + 4;
                if (regs[RS].i64 < 0)
                {
                    pc2 = pc + (SIMM << 2);
                }
                break;
            case OP_REGIMM_BGEZAL:
                /* Debug */
                azDebugDumpState(state);
                getchar();
                regs[31].u64 = pc + 4;
                if (regs[RS].i64 >= 0)
                {
                    pc2 = pc + (SIMM << 2);
                }
                break;
            case OP_REGIMM_BLTZALL:
                /* Debug */
                azDebugDumpState(state);
                getchar();
                regs[31].u64 = pc + 4;
                if (regs[RS].i64 < 0)
                {
                    pc2 = pc + (SIMM << 2);
                }
                else
                {
                    pc += 4;
                    pc2 = pc + 4;
                }
                break;
            case OP_REGIMM_BGEZALL:
                /* Debug */
                azDebugDumpState(state);
                getchar();
                regs[31].u64 = pc + 4;
                if (regs[RS].i64 >= 0)
                {
                    pc2 = pc + (SIMM << 2);
                }
                else
                {
                    pc += 4;
                    pc2 = pc + 4;
                }
                break;
            }
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
            if (regs[RS].i64 <= 0) pc2 = pc + (SIMM << 2);
            break;
        case OP_BGTZ:
            if (regs[RS].i64 > 0) pc2 = pc + (SIMM << 2);
            break;
        case OP_ADDI:
            if (RT) regs[RT].i64 = (int32_t)(regs[RS].u32 + SIMM);
            break;
        case OP_ADDIU:
            if (RT) regs[RT].i64 = (int32_t)(regs[RS].u32 + SIMM);
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
                regs[RT].i64 = (int32_t)(IMM << 16);
            break;
        case OP_COP0:
            switch (RS)
            {
            case OP_COP_MF:
                if (RT) regs[RT].i64 = (int32_t)state->cop0.registers[RD];
                break;
            case OP_COP_DMF:
                if (RT) regs[RT].u64 = state->cop0.registers[RD];
                break;
            case OP_COP_MT:
                state->cop0.registers[RD] = regs[RT].u64;
                break;
            case OP_COP_DMT:
                state->cop0.registers[RD] = regs[RT].u64;
                break;
            case OP_COP_BC:
                break;
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18:
            case 0x19:
            case 0x1a:
            case 0x1b:
            case 0x1c:
            case 0x1d:
            case 0x1e:
            case 0x1f:
                switch (op & 0x3f)
                {
                case OP_CP0_TLBR:
                    break;
                case OP_CP0_TLBWI:
                    break;
                case OP_CP0_TLBWR:
                    break;
                case OP_CP0_TLBP:
                    break;
                case OP_CP0_ERET:
                    printf("ERET\n");
                    if (state->cop0.registers[COP0_REG_STATUS] & 0x04)
                    {
                        pc = state->cop0.registers[COP0_REG_ERROR_EPC];
                        state->cop0.registers[COP0_REG_STATUS] &= ~(0x04);
                    }
                    else
                    {
                        pc = state->cop0.registers[COP0_REG_EPC];
                        state->cop0.registers[COP0_REG_STATUS] &= ~(0x02);
                    }
                    pc2 = pc + 4;
                    break;
                }
                break;
            }
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
                pc += 4;
                pc2 = pc + 4;
            }
            break;
        case OP_BNEL:
            if (regs[RS].u64 != regs[RT].u64)
            {
                pc2 = pc + (SIMM << 2);
            }
            else
            {
                pc += 4;
                pc2 = pc + 4;
            }
            break;
        case OP_BLEZL:
            if (regs[RS].i64 <= 0)
            {
                pc2 = pc + (SIMM << 2);
            }
            else
            {
                pc += 4;
                pc2 = pc + 4;
            }
            break;
        case OP_BGTZL:
            if (regs[RS].i64 > 0)
            {
                pc2 = pc + (SIMM << 2);
            }
            else
            {
                pc += 4;
                pc2 = pc + 4;
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
    state->cpu.hi = hi.u64;
    state->cpu.lo = lo.u64;
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
