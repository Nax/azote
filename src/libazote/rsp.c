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
        op = bswap32(*(uint32_t*)(state->spImem + (pc & 0xfff)));
        pc = pc2 & 0xfff;
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
                if (RD) regs[RD] = regs[RT] << SA;
                break;
            case OP_SPECIAL_SRL:
                if (RD) regs[RD] = regs[RT] >> SA;
                break;
            case OP_SPECIAL_SRA:
                if (RD) regs[RD] = sra32(regs[RT], SA);
                break;
            case OP_SPECIAL_SLLV:
                if (RD) regs[RD] = regs[RT] << (regs[RS] & 0x1f);
                break;
            case OP_SPECIAL_SRLV:
                if (RD) regs[RD] = regs[RT] >> (regs[RS] & 0x1f);
                break;
            case OP_SPECIAL_SRAV:
                if (RD) regs[RD] = sra32(regs[RT], regs[RS] & 0x1f);
                break;
            case OP_SPECIAL_JR:
                pc2 = regs[RS];
                break;
            case OP_SPECIAL_JALR:
                pc2 = regs[RS];
                regs[31] = pc + 4;
                break;
            case OP_SPECIAL_BREAK:
                printf("RSP BREAK\n");
                TRAP;
                break;
            case OP_SPECIAL_ADD:
            case OP_SPECIAL_ADDU:
                if (RD) regs[RD] = regs[RS] + regs[RT];
                break;
            case OP_SPECIAL_SUB:
            case OP_SPECIAL_SUBU:
                if (RD) regs[RD] = regs[RS] - regs[RT];
                break;
            case OP_SPECIAL_AND:
                if (RD) regs[RD] = regs[RS] & regs[RT];
                break;
            case OP_SPECIAL_OR:
                if (RD) regs[RD] = regs[RS] | regs[RT];
                break;
            case OP_SPECIAL_XOR:
                if (RD) regs[RD] = regs[RS] ^ regs[RT];
                break;
            case OP_SPECIAL_NOR:
                if (RD) regs[RD] = ~(regs[RS] | regs[RT]);
                break;
            case OP_SPECIAL_SLT:
                if (RD)
                {
                    if ((int32_t)regs[RS] < (int32_t)regs[RT])
                        regs[RD] = 1;
                    else
                        regs[RD] = 0;
                }
                break;
            case OP_SPECIAL_SLTU:
                if (RD)
                {
                    if (regs[RS] < regs[RT])
                        regs[RD] = 1;
                    else
                        regs[RD] = 0;
                }
                break;
            }
            break;
        case OP_REGIMM:
            switch (RT)
            {
            case OP_REGIMM_BLTZ:
                if ((int32_t)regs[RS] < 0)
                    pc2 = pc + (SIMM << 2);
                break;
            case OP_REGIMM_BGEZ:
                if ((int32_t)regs[RS] >= 0)
                    pc2 = pc + (SIMM << 2);
                break;
            case OP_REGIMM_BLTZAL:
                regs[31] = pc + 4;
                if ((int32_t)regs[RS] < 0)
                    pc2 = pc + (SIMM << 2);
                break;
            case OP_REGIMM_BGEZAL:
                regs[31] = pc + 4;
                if ((int32_t)regs[RS] >= 0)
                    pc2 = pc + (SIMM << 2);
                break;
            }
            break;
        case OP_J:
            pc2 = ((op << 2) & 0xfff);
            break;
        case OP_JAL:
            regs[31] = pc + 4;
            pc2 = ((op << 2) & 0xfff);
            break;
        case OP_BEQ:
            if (regs[RS] == regs[RT])
                pc2 = pc + (SIMM << 2);
            break;
        case OP_BNE:
            if (regs[RS] != regs[RT])
                pc2 = pc + (SIMM << 2);
            break;
        case OP_BLEZ:
            if ((int32_t)regs[RS] <= 0)
                pc2 = pc + (SIMM << 2);
            break;
        case OP_BGTZ:
            if ((int32_t)regs[RS] > 0)
                pc2 = pc + (SIMM << 2);
            break;
        case OP_ADDI:
        case OP_ADDIU:
            if (RT) regs[RT] = regs[RS] + SIMM;
            break;
        case OP_SLTI:
        case OP_SLTIU:
            if (RT)
            {
                if ((int32_t)regs[RS] < SIMM)
                    regs[RT] = 1;
                else
                    regs[RT] = 0;
            }
            break;
        case OP_ANDI:
            if (RT) regs[RT] = regs[RS] & IMM;
            break;
        case OP_ORI:
            if (RT) regs[RT] = regs[RS] | IMM;
            break;
        case OP_XORI:
            if (RT) regs[RT] = regs[RS] ^ IMM;
            break;
        case OP_LUI:
            if (RT) regs[RT] = ((uint32_t)IMM << 16);
            break;
        case OP_COP0:
            switch (RS)
            {
            default:
                TRAP;
                break;
            case OP_COP_MF:
                if (RT) regs[RT] = azRspControlRead(state, RD);
                break;
            case OP_COP_MT:
                azRspControlWrite(state, RD, regs[RT]);
                break;
            }
            break;
        case OP_COP2:
            TRAP;
            break;
        case OP_LB:
            if (RT) regs[RT] = (int32_t)(*(int8_t*)(state->spDmem + ((regs[RS] + SIMM) & 0xfff)));
            break;
        case OP_LH:
            if (RT) regs[RT] = (int32_t)((int16_t)bswap16(*(uint16_t*)(state->spDmem + ((regs[RS] + SIMM) & 0xfff))));
            break;
        case OP_LW:
            if (RT) regs[RT] = bswap32(*(uint32_t*)(state->spDmem + ((regs[RS] + SIMM) & 0xfff)));
            break;
        case OP_LBU:
            if (RT) regs[RT] = *(uint8_t*)(state->spDmem + ((regs[RS] + SIMM) & 0xfff));
            break;
        case OP_LHU:
            if (RT) regs[RT] = bswap16(*(uint16_t*)(state->spDmem + ((regs[RS] + SIMM) & 0xfff)));
            break;
        case OP_SB:
            *(uint8_t*)(state->spDmem + ((regs[RS] + SIMM) & 0xfff)) = regs[RT];
            break;
        case OP_SH:
            *(uint16_t*)(state->spDmem + ((regs[RS] + SIMM) & 0xfff)) = bswap16(regs[RT]);
            break;
        case OP_SW:
            *(uint32_t*)(state->spDmem + ((regs[RS] + SIMM) & 0xfff)) = bswap32(regs[RT]);
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
