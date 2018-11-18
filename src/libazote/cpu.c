#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <libazote/libazote.h>
#include <libazote/mips.h>

#define TRAP    do { printf("CPU TRAP at 0x%016llx    OP: 0x%08x\n", pc, op); getchar(); } while (0)

static uint32_t _runCycles(AzState* state, uint32_t cycles)
{
    uint32_t i;
    uint32_t op;
    uint64_t pc;
    uint64_t pc2;
    uint64_t tmp;
    uint64_t tmp2;
    __uint128_t u128;
    AzReg* regs = (AzReg*)(&state->cpu.registers[0]);
    AzReg hi;
    AzReg lo;

    pc = state->cpu.pc;
    pc2 = state->cpu.pc2;
    //memcpy(regs, state->cpu.registers, 32 * sizeof(AzReg));
    hi.u64 = state->cpu.hi;
    lo.u64 = state->cpu.lo;

    for (i = 0; i < cycles; ++i)
    {
        op = azMemoryRead32(state, pc);
        pc = pc2;
        pc2 += 4;

        switch (op >> 26)
        {
        default:
            TRAP;
            break;
        case OP_SPECIAL:
            switch (op & 0x3f)
            {
            default:
                TRAP;
                break;
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
                TRAP;
                break;
            case OP_SPECIAL_BREAK:
                TRAP;
                break;
            case OP_SPECIAL_SYNC:
                TRAP;
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
                if (RD) regs[RD].u64 = regs[RT].u64 << (regs[RS].u64 & 0x3f);
                break;
            case OP_SPECIAL_DSRLV:
                if (RD) regs[RD].u64 = regs[RT].u64 >> (regs[RS].u64 & 0x3f);
                break;
            case OP_SPECIAL_DSRAV:
                if (RD) regs[RD].u64 = sra64(regs[RT].i64, regs[RS].u64 & 0x3f);
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
                u128 = (__int128_t)regs[RS].i64 * (__int128_t)regs[RT].i64;
                hi.u64 = (u128 >> 64);
                lo.u64 = (u128 & 0xffffffffffffffff);
                break;
            case OP_SPECIAL_DMULTU:
                u128 = (__uint128_t)regs[RS].u64 * (__uint128_t)regs[RT].u64;
                hi.u64 = (u128 >> 64);
                lo.u64 = (u128 & 0xffffffffffffffff);
                break;
            case OP_SPECIAL_DDIV:
                lo.u64 = regs[RS].i64 / regs[RT].i64;
                hi.u64 = regs[RS].i64 % regs[RT].i64;
                break;
            case OP_SPECIAL_DDIVU:
                lo.u64 = regs[RS].u64 / regs[RT].u64;
                hi.u64 = regs[RS].u64 % regs[RT].u64;
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
                if (RD) regs[RD].u64 = regs[RS].u64 + regs[RT].u64;
                break;
            case OP_SPECIAL_DADDU:
                if (RD) regs[RD].u64 = regs[RS].u64 + regs[RT].u64;
                break;
            case OP_SPECIAL_DSUB:
                if (RD) regs[RD].u64 = regs[RS].u64 - regs[RT].u64;
                break;
            case OP_SPECIAL_DSUBU:
                if (RD) regs[RD].u64 = regs[RS].u64 - regs[RT].u64;
                break;
            case OP_SPECIAL_TGE:
                TRAP;
                break;
            case OP_SPECIAL_TGEU:
                TRAP;
                break;
            case OP_SPECIAL_TLT:
                TRAP;
                break;
            case OP_SPECIAL_TLTU:
                TRAP;
                break;
            case OP_SPECIAL_TEQ:
                TRAP;
                break;
            case OP_SPECIAL_TNE:
                TRAP;
                break;
            case OP_SPECIAL_DSLL:
                if (RD) regs[RD].u64 = regs[RT].u64 << SA;
                break;
            case OP_SPECIAL_DSRL:
                if (RD) regs[RD].u64 = regs[RT].u64 >> SA;
                break;
            case OP_SPECIAL_DSRA:
                if (RD) regs[RD].u64 = sra64(regs[RT].i64, SA);
                break;
            case OP_SPECIAL_DSLL32:
                if (RD) regs[RD].u64 = regs[RT].u64 << (SA + 32);
                break;
            case OP_SPECIAL_DSRL32:
                if (RD) regs[RD].u64 = regs[RT].u64 >> (SA + 32);
                break;
            case OP_SPECIAL_DSRA32:
                if (RD) regs[RD].u64 = sra64(regs[RT].i64, SA + 32);
                break;
            }
            break;
        case OP_REGIMM:
            switch (RT)
            {
            default:
                TRAP;
                break;
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
                TRAP;
                break;
            case OP_REGIMM_TGEIU:
                TRAP;
                break;
            case OP_REGIMM_TLTI:
                TRAP;
                break;
            case OP_REGIMM_TLTIU:
                TRAP;
                break;
            case OP_REGIMM_TEQI:
                TRAP;
                break;
            case OP_REGIMM_TNEI:
                TRAP;
                break;
            case OP_REGIMM_BLTZAL:
                regs[31].u64 = pc + 4;
                if (regs[RS].i64 < 0)
                {
                    pc2 = pc + (SIMM << 2);
                }
                break;
            case OP_REGIMM_BGEZAL:
                regs[31].u64 = pc + 4;
                if (regs[RS].i64 >= 0)
                {
                    pc2 = pc + (SIMM << 2);
                }
                break;
            case OP_REGIMM_BLTZALL:
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
            default:
                TRAP;
                break;
            case OP_COP_MF:
                if (RT) regs[RT].i64 = (int32_t)state->cop0.registers[RD];
                break;
            case OP_COP_DMF:
                if (RT) regs[RT].u64 = state->cop0.registers[RD];
                break;
            case OP_COP_MT:
                state->cop0.registers[RD] = regs[RT].u64;
                if (RD == COP0_REG_COMPARE)
                {
                    i = 0;
                    state->cop0.registers[COP0_REG_CAUSE] &= ~(1 << 15);
                    state->cop0.registers[COP0_REG_COUNT] = 0;
                    goto end;
                }
                break;
            case OP_COP_DMT:
                state->cop0.registers[RD] = regs[RT].u64;
                if (RD == COP0_REG_COMPARE)
                {
                    i = 0;
                    state->cop0.registers[COP0_REG_CAUSE] &= ~(1 << 15);
                    state->cop0.registers[COP0_REG_COUNT] = 0;
                    goto end;
                }
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
                default:
                    TRAP;
                    break;
                case OP_CP0_TLBR:
                    // TRAP;
                    break;
                case OP_CP0_TLBWI:
                    tmp = state->cop0.registers[COP0_REG_INDEX] & 0x1f;
                    state->tlb.lo0[tmp] = state->cop0.registers[COP0_REG_ENTRYLO0];
                    state->tlb.lo1[tmp] = state->cop0.registers[COP0_REG_ENTRYLO1];
                    state->tlb.hi[tmp] = state->cop0.registers[COP0_REG_ENTRYHI];
                    state->tlb.mask[tmp] = state->cop0.registers[COP0_REG_PAGE_MASK];
                    break;
                case OP_CP0_TLBWR:
                    TRAP;
                    break;
                case OP_CP0_TLBP:
                    // TRAP;
                    break;
                case OP_CP0_ERET:
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
            switch (RS)
            {
            default:
                TRAP;
                break;
            case OP_COP_MF:
                if (RT) regs[RT].i64 = state->cop1.registers[RD].i32;
                break;
            case OP_COP_DMF:
                if (RT) regs[RT].u64 = state->cop1.registers[RD].u64;
                break;
            case OP_COP_CF:
                if (RT)
                {
                    if (RD == 0)
                        regs[RT].u64 = state->cop1.fcr0;
                    else if (RD == 31)
                        regs[RT].u64 = state->cop1.fcr31;
                }
                break;
            case OP_COP_MT:
                state->cop1.registers[RD].u32 = regs[RT].u32;
                break;
            case OP_COP_DMT:
                if (RD == COP0_REG_COMPARE)
                {
                    printf("Compare: %d\n", regs[RT].u32);
                    exit(62);
                }
                state->cop1.registers[RD].u64 = regs[RT].u64;
                break;
            case OP_COP_CT:
                if (RD == 0)
                    state->cop1.fcr0 = regs[RT].u32;
                else if (RD == 31)
                    state->cop1.fcr31 = regs[RT].u32;
                break;
            case OP_COP_BC:
                switch (RT)
                {
                default:
                    TRAP;
                    break;
                case COP1_BC_BCF:
                    if ((state->cop1.fcr31 & (1 << 23)) == 0)
                    {
                        pc2 = pc + (SIMM << 2);
                    }
                    break;
                case COP1_BC_BCT:
                    if ((state->cop1.fcr31 & (1 << 23)) != 0)
                    {
                        pc2 = pc + (SIMM << 2);
                    }
                    break;
                case COP1_BC_BCFL:
                    if ((state->cop1.fcr31 & (1 << 23)) == 0)
                    {
                        pc2 = pc + (SIMM << 2);
                    }
                    else
                    {
                        pc += 4;
                        pc2 += 4;
                    }
                    break;
                case COP1_BC_BCTL:
                    if ((state->cop1.fcr31 & (1 << 23)) != 0)
                    {
                        pc2 = pc + (SIMM << 2);
                    }
                    else
                    {
                        pc += 4;
                        pc2 += 4;
                    }
                    break;
                }
                break;
            case OP_COP1_S:
                switch (op & 0x3f)
                {
                default:
                    TRAP;
                    break;
                case OP_CP1_ADD:
                    state->cop1.registers[FD].f = state->cop1.registers[FS].f + state->cop1.registers[FT].f;
                    break;
                case OP_CP1_SUB:
                    state->cop1.registers[FD].f = state->cop1.registers[FS].f - state->cop1.registers[FT].f;
                    break;
                case OP_CP1_MUL:
                    state->cop1.registers[FD].f = state->cop1.registers[FS].f * state->cop1.registers[FT].f;
                    break;
                case OP_CP1_DIV:
                    state->cop1.registers[FD].f = state->cop1.registers[FS].f / state->cop1.registers[FT].f;
                    break;
                case OP_CP1_SQRT:
                    state->cop1.registers[FD].f = sqrtf(state->cop1.registers[FS].f);
                    break;
                case OP_CP1_ABS:
                    state->cop1.registers[FD].f = fabsf(state->cop1.registers[FS].f);
                    break;
                case OP_CP1_MOV:
                    state->cop1.registers[FD].f = state->cop1.registers[FS].f;
                    break;
                case OP_CP1_NEG:
                    state->cop1.registers[FD].f = -(state->cop1.registers[FS].f);
                    break;
                case OP_CP1_ROUND_L:
                    state->cop1.registers[FD].u64 = fpS2L(roundf(state->cop1.registers[FS].f));
                    break;
                case OP_CP1_TRUNC_L:
                    state->cop1.registers[FD].u64 = fpS2L(truncf(state->cop1.registers[FS].f));
                    break;
                case OP_CP1_CEIL_L:
                    state->cop1.registers[FD].u64 = fpS2L(ceilf(state->cop1.registers[FS].f));
                    break;
                case OP_CP1_FLOOR_L:
                    state->cop1.registers[FD].u64 = fpS2L(floorf(state->cop1.registers[FS].f));
                    break;
                case OP_CP1_ROUND_W:
                    state->cop1.registers[FD].u32 = fpS2W(roundf(state->cop1.registers[FS].f));
                    break;
                case OP_CP1_TRUNC_W:
                    state->cop1.registers[FD].u32 = fpS2W(truncf(state->cop1.registers[FS].f));
                    break;
                case OP_CP1_CEIL_W:
                    state->cop1.registers[FD].u32 = fpS2W(ceilf(state->cop1.registers[FS].f));
                    break;
                case OP_CP1_FLOOR_W:
                    state->cop1.registers[FD].u32 = fpS2W(floorf(state->cop1.registers[FS].f));
                    break;
                case OP_CP1_CVT_D:
                    state->cop1.registers[FD].d = (double)state->cop1.registers[FS].f;
                    break;
                case OP_CP1_CVT_W:
                    state->cop1.registers[FD].u32 = fpS2W(state->cop1.registers[FS].f);
                    break;
                case OP_CP1_CVT_L:
                    state->cop1.registers[FD].u64 = fpS2L(state->cop1.registers[FS].f);
                    break;
                case OP_CP1_C_F:
                    state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_UN:
                    if (isnan(state->cop1.registers[FS].f)
                     || isnan(state->cop1.registers[FT].f))
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_EQ:
                    if (state->cop1.registers[FS].f == state->cop1.registers[FT].f)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_UEQ:
                    if (isnan(state->cop1.registers[FS].f)
                     || isnan(state->cop1.registers[FT].f)
                     || state->cop1.registers[FS].f == state->cop1.registers[FT].f)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_OLT:
                    if (state->cop1.registers[FS].f < state->cop1.registers[FT].f)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_ULT:
                    if (isnan(state->cop1.registers[FS].f)
                     || isnan(state->cop1.registers[FT].f)
                     || state->cop1.registers[FS].f < state->cop1.registers[FT].f)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_OLE:
                    if (state->cop1.registers[FS].f == state->cop1.registers[FT].f
                     || state->cop1.registers[FS].f < state->cop1.registers[FT].f)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_ULE:
                    if (isnan(state->cop1.registers[FS].f)
                     || isnan(state->cop1.registers[FT].f)
                     || state->cop1.registers[FS].f == state->cop1.registers[FT].f
                     || state->cop1.registers[FS].f < state->cop1.registers[FT].f)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_SF:
                    state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_NGLE:
                    if (isnan(state->cop1.registers[FS].f)
                     || isnan(state->cop1.registers[FT].f))
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_SEQ:
                    if (state->cop1.registers[FS].f == state->cop1.registers[FT].f)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_NGL:
                    if (isnan(state->cop1.registers[FS].f)
                     || isnan(state->cop1.registers[FT].f)
                     || state->cop1.registers[FS].f == state->cop1.registers[FT].f)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_LT:
                    if (state->cop1.registers[FS].f < state->cop1.registers[FT].f)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_NGE:
                    if (isnan(state->cop1.registers[FS].f)
                     || isnan(state->cop1.registers[FT].f)
                     || state->cop1.registers[FS].f < state->cop1.registers[FT].f)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_LE:
                    if (state->cop1.registers[FS].f == state->cop1.registers[FT].f
                     || state->cop1.registers[FS].f < state->cop1.registers[FT].f)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_NGT:
                    if (isnan(state->cop1.registers[FS].f)
                     || isnan(state->cop1.registers[FT].f)
                     || state->cop1.registers[FS].f == state->cop1.registers[FT].f
                     || state->cop1.registers[FS].f < state->cop1.registers[FT].f)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                }
                break;
            case OP_COP1_D:
                switch (op & 0x3f)
                {
                default:
                    TRAP;
                    break;
                case OP_CP1_ADD:
                    state->cop1.registers[FD].d = state->cop1.registers[FS].d + state->cop1.registers[FT].d;
                    break;
                case OP_CP1_SUB:
                    state->cop1.registers[FD].d = state->cop1.registers[FS].d - state->cop1.registers[FT].d;
                    break;
                case OP_CP1_MUL:
                    state->cop1.registers[FD].d = state->cop1.registers[FS].d * state->cop1.registers[FT].d;
                    break;
                case OP_CP1_DIV:
                    state->cop1.registers[FD].d = state->cop1.registers[FS].d / state->cop1.registers[FT].d;
                    break;
                case OP_CP1_SQRT:
                    state->cop1.registers[FD].d = sqrt(state->cop1.registers[FS].d);
                    break;
                case OP_CP1_ABS:
                    state->cop1.registers[FD].d = fabs(state->cop1.registers[FS].d);
                    break;
                case OP_CP1_MOV:
                    state->cop1.registers[FD].d = state->cop1.registers[FS].d;
                    break;
                case OP_CP1_NEG:
                    state->cop1.registers[FD].d = -(state->cop1.registers[FS].d);
                    break;
                case OP_CP1_ROUND_L:
                    state->cop1.registers[FD].u64 = fpD2L(round(state->cop1.registers[FS].d));
                    break;
                case OP_CP1_TRUNC_L:
                    state->cop1.registers[FD].u64 = fpD2L(trunc(state->cop1.registers[FS].d));
                    break;
                case OP_CP1_CEIL_L:
                    state->cop1.registers[FD].u64 = fpD2L(ceil(state->cop1.registers[FS].d));
                    break;
                case OP_CP1_FLOOR_L:
                    state->cop1.registers[FD].u64 = fpD2L(floor(state->cop1.registers[FS].d));
                    break;
                case OP_CP1_ROUND_W:
                    state->cop1.registers[FD].u32 = fpD2W(round(state->cop1.registers[FS].d));
                    break;
                case OP_CP1_TRUNC_W:
                    state->cop1.registers[FD].u32 = fpD2W(trunc(state->cop1.registers[FS].d));
                    break;
                case OP_CP1_CEIL_W:
                    state->cop1.registers[FD].u32 = fpD2W(ceil(state->cop1.registers[FS].d));
                    break;
                case OP_CP1_FLOOR_W:
                    state->cop1.registers[FD].u32 = fpD2W(floor(state->cop1.registers[FS].d));
                    break;
                case OP_CP1_CVT_S:
                    state->cop1.registers[FD].f = (float)state->cop1.registers[FS].d;
                    break;
                case OP_CP1_CVT_W:
                    state->cop1.registers[FD].u32 = fpD2W(state->cop1.registers[FS].d);
                    break;
                case OP_CP1_CVT_L:
                    state->cop1.registers[FD].u64 = fpD2L(state->cop1.registers[FS].d);
                    break;
                case OP_CP1_C_F:
                    state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_UN:
                    if (isnan(state->cop1.registers[FS].d)
                     || isnan(state->cop1.registers[FT].d))
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_EQ:
                    if (state->cop1.registers[FS].d == state->cop1.registers[FT].d)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_UEQ:
                    if (isnan(state->cop1.registers[FS].d)
                     || isnan(state->cop1.registers[FT].d)
                     || state->cop1.registers[FS].d == state->cop1.registers[FT].d)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_OLT:
                    if (state->cop1.registers[FS].d < state->cop1.registers[FT].d)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_ULT:
                    if (isnan(state->cop1.registers[FS].d)
                     || isnan(state->cop1.registers[FT].d)
                     || state->cop1.registers[FS].d < state->cop1.registers[FT].d)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_OLE:
                    if (state->cop1.registers[FS].d == state->cop1.registers[FT].d
                     || state->cop1.registers[FS].d < state->cop1.registers[FT].d)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_ULE:
                    if (isnan(state->cop1.registers[FS].d)
                     || isnan(state->cop1.registers[FT].d)
                     || state->cop1.registers[FS].d == state->cop1.registers[FT].d
                     || state->cop1.registers[FS].d < state->cop1.registers[FT].d)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_SF:
                    state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_NGLE:
                    if (isnan(state->cop1.registers[FS].d)
                     || isnan(state->cop1.registers[FT].d))
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_SEQ:
                    if (state->cop1.registers[FS].d == state->cop1.registers[FT].d)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_NGL:
                    if (isnan(state->cop1.registers[FS].d)
                     || isnan(state->cop1.registers[FT].d)
                     || state->cop1.registers[FS].d == state->cop1.registers[FT].d)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_LT:
                    if (state->cop1.registers[FS].d < state->cop1.registers[FT].d)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_NGE:
                    if (isnan(state->cop1.registers[FS].d)
                     || isnan(state->cop1.registers[FT].d)
                     || state->cop1.registers[FS].d < state->cop1.registers[FT].d)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_LE:
                    if (state->cop1.registers[FS].d == state->cop1.registers[FT].d
                     || state->cop1.registers[FS].d < state->cop1.registers[FT].d)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                case OP_CP1_C_NGT:
                    if (isnan(state->cop1.registers[FS].d)
                     || isnan(state->cop1.registers[FT].d)
                     || state->cop1.registers[FS].d == state->cop1.registers[FT].d
                     || state->cop1.registers[FS].d < state->cop1.registers[FT].d)
                        state->cop1.fcr31 |= (1 << 23);
                    else
                        state->cop1.fcr31 &= ~(1 << 23);
                    break;
                }
                break;
            case OP_COP1_W:
                switch (op & 0x3f)
                {
                default:
                    TRAP;
                    break;
                case OP_CP1_CVT_S:
                    state->cop1.registers[FD].f = fpW2S(state->cop1.registers[FS].u32);
                    break;
                case OP_CP1_CVT_D:
                    state->cop1.registers[FD].d = fpW2D(state->cop1.registers[FS].u32);
                    break;
                }
                break;
            case OP_COP1_L:
                switch (op & 0x3f)
                {
                default:
                    TRAP;
                    break;
                case OP_CP1_CVT_S:
                    state->cop1.registers[FD].f = fpL2S(state->cop1.registers[FS].u64);
                    break;
                case OP_CP1_CVT_D:
                    state->cop1.registers[FD].d = fpL2D(state->cop1.registers[FS].u64);
                    break;
                }
                break;
            }
            break;
        case OP_COP2:
            TRAP;
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
            TRAP;
            break;
        case OP_LDR:
            TRAP;
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
            if (RT)
            {
                tmp = regs[RS].u64 + SIMM;
                tmp2 = (uint32_t)azMemoryRead32(state, tmp & ~0x3);
                switch (tmp & 0x03)
                {
                case 0x00:
                    tmp = tmp2;
                    break;
                case 0x01:
                    tmp = (tmp2 << 8) | (regs[RT].u32 & 0x000000ff);
                    break;
                case 0x02:
                    tmp = (tmp2 << 16) | (regs[RT].u32 & 0x0000ffff);
                    break;
                case 0x03:
                    tmp = (tmp2 << 24) | (regs[RT].u32 & 0x00ffffff);
                    break;
                }
                regs[RT].i64 = (int32_t)tmp;
            }
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
            if (RT)
            {
                tmp = regs[RS].u64 + SIMM;
                tmp2 = (uint32_t)azMemoryRead32(state, tmp & ~0x3);
                switch (tmp & 0x03)
                {
                case 0x00:
                    tmp = (tmp2 >> 24) | (regs[RT].u32 & 0xffffff00);
                    break;
                case 0x01:
                    tmp = (tmp2 >> 16) | (regs[RT].u32 & 0xffff0000);
                    break;
                case 0x02:
                    tmp = (tmp2 >> 8) | (regs[RT].u32 & 0xff000000);
                    break;
                case 0x03:
                    tmp = tmp2;
                    break;
                }
                regs[RT].i64 = (int32_t)tmp;
            }
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
            tmp = regs[RS].u64 + SIMM;
            tmp2 = (uint32_t)azMemoryRead32(state, tmp & ~0x3);
            switch (tmp & 0x03)
            {
            case 0x00:
                azMemoryWrite32(state, tmp & ~0x3, tmp2);
                break;
            case 0x01:
                azMemoryWrite32(state, tmp & ~0x3, (regs[RT].u32 >> 8) | (tmp2 & 0xff000000));
                break;
            case 0x02:
                azMemoryWrite32(state, tmp & ~0x3, (regs[RT].u32 >> 16) | (tmp2 & 0xffff0000));
                break;
            case 0x03:
                azMemoryWrite32(state, tmp & ~0x3, (regs[RT].u32 >> 24) | (tmp2 & 0xffffff00));
                break;
            }
            break;
        case OP_SW:
            azMemoryWrite32(state, regs[RS].u64 + SIMM, regs[RT].u64);
            break;
        case OP_SDL:
            TRAP;
            break;
        case OP_SDR:
            TRAP;
            break;
        case OP_SWR:
            tmp = regs[RS].u64 + SIMM;
            tmp2 = (uint32_t)azMemoryRead32(state, tmp & ~0x3);
            switch (tmp & 0x03)
            {
            case 0x00:
                azMemoryWrite32(state, tmp & ~0x3, (regs[RT].u32 << 24) | (tmp2 & 0x00ffffff));
                break;
            case 0x01:
                azMemoryWrite32(state, tmp & ~0x3, (regs[RT].u32 << 16) | (tmp2 & 0x0000ffff));
                break;
            case 0x02:
                azMemoryWrite32(state, tmp & ~0x3, (regs[RT].u32 << 8) | (tmp2 & 0x000000ff));
                break;
            case 0x03:
                azMemoryWrite32(state, tmp & ~0x3, tmp2);
                break;
            }
            break;
        case OP_CACHE:
            // TRAP;
            break;
        case OP_LL:
            TRAP;
            break;
        case OP_LWC1:
            state->cop1.registers[RT].u32 = azMemoryRead32(state, regs[RS].u64 + SIMM);
            break;
        case OP_LLD:
            TRAP;
            break;
        case OP_LDC1:
            state->cop1.registers[RT].u64 = azMemoryRead64(state, regs[RS].u64 + SIMM);
            break;
        case OP_LD:
            if (RT) regs[RT].u64 = azMemoryRead64(state, regs[RS].u64 + SIMM);
            break;
        case OP_SC:
            TRAP;
            break;
        case OP_SWC1:
            azMemoryWrite32(state, regs[RS].u64 + SIMM, state->cop1.registers[RT].u32);
            break;
        case OP_SCD:
            TRAP;
            break;
        case OP_SDC1:
            azMemoryWrite64(state, regs[RS].u64 + SIMM, state->cop1.registers[RT].u64);
            break;
        case OP_SD:
            azMemoryWrite64(state, regs[RS].u64 + SIMM, regs[RT].u64);
            break;
        }

        /* Handle Interrupts */
        tmp = state->cop0.registers[COP0_REG_STATUS];
        if ((tmp & 0x01) == 0 || (tmp & 0x06) != 0)
            continue;
        tmp2 = (state->cop0.registers[COP0_REG_CAUSE] >> 8) & 0xff;
        tmp2 &= ((tmp >> 8) & 0xff);
        if (tmp2)
        {
            int bd = (pc2 != pc + 4);
            state->cop0.registers[COP0_REG_STATUS] |= 0x02;
            state->cop0.registers[COP0_REG_EPC] = pc;
            if (bd)
                state->cop0.registers[COP0_REG_EPC] -= 4;
            pc = 0xffffffff80000180;
            pc2 = pc + 4;
        }
    }

end:
    state->cpu.pc = pc;
    state->cpu.pc2 = pc2;
    //memcpy(state->cpu.registers, regs, 32 * sizeof(AzReg));
    state->cpu.hi = hi.u64;
    state->cpu.lo = lo.u64;
    return i;
}

static inline uint64_t _getTimeNano()
{
    return clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
}

void* azCpuWorkerMain(void* s)
{
    AzState* state = (AzState*)s;

    static const uint32_t granularity = 1024;
    static const uint64_t kPeriod = 1666666600;
    uint64_t now;
    uint64_t referenceTime;
    uint64_t baseTime;
    uint64_t dt;
    uint64_t cycles;
    uint64_t slice;

    pthread_setname_np("CPU Worker");

    cycles = 0;
    referenceTime = _getTimeNano();
    baseTime = _getTimeNano();

    for (;;)
    {
        azWorkerBarrier(&state->cpuWorker);
        if (state->cop0.registers[COP0_REG_COMPARE] > state->cop0.registers[COP0_REG_COUNT]
            && (state->cop0.registers[COP0_REG_COMPARE] - state->cop0.registers[COP0_REG_COUNT]) < granularity)
        {
            slice = _runCycles(state, state->cop0.registers[COP0_REG_COMPARE] - state->cop0.registers[COP0_REG_COUNT]);
        }
        else
        {
            slice = _runCycles(state, granularity);
        }
        cycles += slice;
        state->cop0.registers[COP0_REG_COUNT] += slice;
        if (state->cop0.registers[COP0_REG_COUNT] == state->cop0.registers[COP0_REG_COMPARE] && state->cop0.registers[COP0_REG_COUNT] != 0)
        {
            state->cop0.registers[COP0_REG_CAUSE] |= (1 << 15);
        }
        now = _getTimeNano();
        dt = now - referenceTime;
        if (dt >= kPeriod)
        {
            referenceTime += kPeriod;
            azRcpRaiseInterrupt(state, RCP_INTR_VI);
        }
        if ((now - baseTime) >= 1000000000)
        {
            double instrPerSecond = (double)cycles / ((double)(now - baseTime) * 1e-9);
            printf("CPU Vi/s: %.2fM   (PC: 0x%016llx)\n", instrPerSecond / 1000000, state->cpu.pc);
            baseTime = now;
            cycles = 0;
        }
    }
}
