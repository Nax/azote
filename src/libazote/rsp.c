#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <libazote/libazote.h>
#include <libazote/mips.h>

#define TRAP    do { printf("RSP TRAP at 0x%04x    OP: 0x%08x\n", pc, op); getchar(); } while (0)

static void _runCycles(AzState* state, uint32_t cycles)
{
    uint32_t op;
    uint16_t pc;
    uint16_t pc2;
    AzReg* regs;

    regs = state->rsp.registers;
    pc = state->rsp.pc;
    pc2 = state->rsp.pc2;

    for (uint32_t i = 0; i < cycles; ++i)
    {
        op = *(uint32_t*)(state->spImem + (pc & 0xfff));
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
                TRAP;
                break;
            case OP_SPECIAL_SRL:
                TRAP;
                break;
            case OP_SPECIAL_SRA:
                TRAP;
                break;
            case OP_SPECIAL_SLLV:
                TRAP;
                break;
            case OP_SPECIAL_SRLV:
                TRAP;
                break;
            case OP_SPECIAL_SRAV:
                TRAP;
                break;
            case OP_SPECIAL_JR:
                TRAP;
                break;
            case OP_SPECIAL_JALR:
                TRAP;
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
                TRAP;
                break;
            case OP_SPECIAL_MTHI:
                TRAP;
                break;
            case OP_SPECIAL_MFLO:
                TRAP;
                break;
            case OP_SPECIAL_MTLO:
                TRAP;
                break;
            case OP_SPECIAL_DSLLV:
                TRAP;
                break;
            case OP_SPECIAL_DSRLV:
                TRAP;
                break;
            case OP_SPECIAL_DSRAV:
                TRAP;
                break;
            case OP_SPECIAL_MULT:
                TRAP;
                break;
            case OP_SPECIAL_MULTU:
                TRAP;
                break;
            case OP_SPECIAL_DIV:
                TRAP;
                break;
            case OP_SPECIAL_DIVU:
                TRAP;
                break;
            case OP_SPECIAL_DMULT:
                TRAP;
                break;
            case OP_SPECIAL_DMULTU:
                TRAP;
                break;
            case OP_SPECIAL_DDIV:
                TRAP;
                break;
            case OP_SPECIAL_DDIVU:
                TRAP;
                break;
            case OP_SPECIAL_ADD:
                TRAP;
                break;
            case OP_SPECIAL_ADDU:
                TRAP;
                break;
            case OP_SPECIAL_SUB:
                TRAP;
                break;
            case OP_SPECIAL_SUBU:
                TRAP;
                break;
            case OP_SPECIAL_AND:
                TRAP;
                break;
            case OP_SPECIAL_OR:
                TRAP;
                break;
            case OP_SPECIAL_XOR:
                TRAP;
                break;
            case OP_SPECIAL_NOR:
                TRAP;
                break;
            case OP_SPECIAL_SLT:
                TRAP;
                break;
            case OP_SPECIAL_SLTU:
                TRAP;
                break;
            case OP_SPECIAL_DADD:
                TRAP;
                break;
            case OP_SPECIAL_DADDU:
                TRAP;
                break;
            case OP_SPECIAL_DSUB:
                TRAP;
                break;
            case OP_SPECIAL_DSUBU:
                TRAP;
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
                TRAP;
                break;
            case OP_SPECIAL_DSRL:
                TRAP;
                break;
            case OP_SPECIAL_DSRA:
                TRAP;
                break;
            case OP_SPECIAL_DSLL32:
                TRAP;
                break;
            case OP_SPECIAL_DSRL32:
                TRAP;
                break;
            case OP_SPECIAL_DSRA32:
                TRAP;
                break;
            }
            break;
        case OP_REGIMM:
            switch (RT)
            {
            case OP_REGIMM_BLTZ:
                TRAP;
                break;
            case OP_REGIMM_BGEZ:
                TRAP;
                break;
            case OP_REGIMM_BLTZL:
                TRAP;
                break;
            case OP_REGIMM_BGEZL:
                TRAP;
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
                TRAP;
                break;
            case OP_REGIMM_BGEZAL:
                TRAP;
                break;
            case OP_REGIMM_BLTZALL:
                TRAP;
                break;
            case OP_REGIMM_BGEZALL:
                TRAP;
                break;
            }
            break;
        case OP_J:
            TRAP;
            break;
        case OP_JAL:
            TRAP;
            break;
        case OP_BEQ:
            TRAP;
            break;
        case OP_BNE:
            TRAP;
            break;
        case OP_BLEZ:
            TRAP;
            break;
        case OP_BGTZ:
            TRAP;
            break;
        case OP_ADDI:
            TRAP;
            break;
        case OP_ADDIU:
            TRAP;
            break;
        case OP_SLTI:
            TRAP;
            break;
        case OP_SLTIU:
            TRAP;
            break;
        case OP_ANDI:
            TRAP;
            break;
        case OP_ORI:
            TRAP;
            break;
        case OP_XORI:
            TRAP;
            break;
        case OP_LUI:
            TRAP;
            break;
        case OP_COP0:
            TRAP;
            break;
        case OP_COP1:
            TRAP;
            break;
        case OP_COP2:
            TRAP;
            break;
        case OP_BEQL:
            TRAP;
            break;
        case OP_BNEL:
            TRAP;
            break;
        case OP_BLEZL:
            TRAP;
            break;
        case OP_BGTZL:
            TRAP;
            break;
        case OP_DADDI:
            TRAP;
            break;
        case OP_DADDIU:
            TRAP;
            break;
        case OP_LDL:
            TRAP;
            break;
        case OP_LDR:
            TRAP;
            break;
        case OP_LB:
            TRAP;
            break;
        case OP_LH:
            TRAP;
            break;
        case OP_LWL:
            TRAP;
            break;
        case OP_LW:
            TRAP;
            break;
        case OP_LBU:
            TRAP;
            break;
        case OP_LHU:
            TRAP;
            break;
        case OP_LWR:
            TRAP;
            break;
        case OP_LWU:
            TRAP;
            break;
        case OP_SB:
            TRAP;
            break;
        case OP_SH:
            TRAP;
            break;
        case OP_SWL:
            TRAP;
            break;
        case OP_SW:
            TRAP;
            break;
        case OP_SDL:
            TRAP;
            break;
        case OP_SDR:
            TRAP;
            break;
        case OP_SWR:
            TRAP;
            break;
        case OP_CACHE:
            TRAP;
            break;
        case OP_LL:
            TRAP;
            break;
        case OP_LWC1:
            TRAP;
            break;
        case OP_LLD:
            TRAP;
            break;
        case OP_LDC1:
            TRAP;
            break;
        case OP_LD:
            TRAP;
            break;
        case OP_SC:
            TRAP;
            break;
        case OP_SWC1:
            TRAP;
            break;
        case OP_SCD:
            TRAP;
            break;
        case OP_SDC1:
            TRAP;
            break;
        case OP_SD:
            TRAP;
            break;
        }
    }

    state->rsp.pc = pc;
    state->rsp.pc2 = pc2;
}

static inline uint64_t _getTimeNano()
{
    return clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
}

void azRunRSP(AzState* state)
{
    static const uint32_t granularity = 1024;
    uint64_t now;
    uint64_t baseTime;
    uint64_t cycles;

    cycles = 0;
    baseTime = _getTimeNano();
    for (;;)
    {
        _runCycles(state, granularity);
        cycles += granularity;
        now = _getTimeNano();
        if ((now - baseTime) >= 1000000000)
        {
            double instrPerSecond = (double)cycles / ((double)(now - baseTime) * 1e-9);
            printf("RSP Vi/s: %.2fM   (PC: 0x%04x)\n", instrPerSecond / 1000000, state->rsp.pc);
            baseTime = now;
            cycles = 0;
        }
    }
}
