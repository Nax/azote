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
    uint32_t* regs;

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
            case OP_SPECIAL_BREAK:
                TRAP;
                break;
            case OP_SPECIAL_ADD:
            case OP_SPECIAL_ADDU:
                TRAP;
                break;
            case OP_SPECIAL_SUB:
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
            case OP_REGIMM_BLTZAL:
                TRAP;
                break;
            case OP_REGIMM_BGEZAL:
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
        case OP_ADDIU:
            TRAP;
            break;
        case OP_SLTI:
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
        case OP_COP2:
            TRAP;
            break;
        case OP_LB:
            TRAP;
            break;
        case OP_LH:
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
        case OP_SB:
            TRAP;
            break;
        case OP_SH:
            TRAP;
            break;
        case OP_SW:
            TRAP;
            break;
        case OP_CACHE:
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
