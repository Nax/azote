#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <libazote/libazote.h>
#include <libazote/mips.h>

#define TRAP    do { printf("RSP TRAP at 0x%04x    OP: 0x%08x\n", pc, op); getchar(); } while (0)

static void* bswapLoad128(void* restrict dst, uint8_t base, void* restrict src, size_t len)
{
    char* cDst = (char*)dst;
    char* cSrc = (char*)src;
    for (size_t i = 0; i < len; ++i)
        cDst[15 - base - i] = cSrc[i];
    return cDst;
}

static void* bswapStore128(void* restrict dst, uint8_t base, void* restrict src, size_t len)
{
    char* cDst = (char*)dst;
    char* cSrc = (char*)src;
    for (size_t i = 0; i < len; ++i)
        cDst[i] = cSrc[15 - base - i];
    return cDst;
}

static void vStore(AzState* state, uint8_t r, __m128i value)
{
    _mm_store_si128(&state->rsp.vregs[r].vi, value);
}

static __m128i vLoad(AzState* state, uint8_t r)
{
    return _mm_load_si128((__m128i*)&state->rsp.vregs[r]);
}

static __m128i vLoadE(AzState* state, uint8_t r, uint8_t e)
{
    switch (e)
    {
    case 0x00:
        return _mm_load_si128((__m128i*)&state->rsp.vregs[r]);
    case 0x02:
        return _mm_set1_epi64x(state->rsp.vregs[r].u64[1]);
    case 0x03:
        return _mm_set1_epi64x(state->rsp.vregs[r].u64[0]);
    case 0x04:
        return _mm_set1_epi32(state->rsp.vregs[r].u32[3]);
    case 0x05:
        return _mm_set1_epi32(state->rsp.vregs[r].u32[2]);
    case 0x06:
        return _mm_set1_epi32(state->rsp.vregs[r].u32[1]);
    case 0x07:
        return _mm_set1_epi32(state->rsp.vregs[r].u32[0]);
    case 0x08:
        return _mm_set1_epi16(state->rsp.vregs[r].u16[7]);
    case 0x09:
        return _mm_set1_epi16(state->rsp.vregs[r].u16[6]);
    case 0x0a:
        return _mm_set1_epi16(state->rsp.vregs[r].u16[5]);
    case 0x0b:
        return _mm_set1_epi16(state->rsp.vregs[r].u16[4]);
    case 0x0c:
        return _mm_set1_epi16(state->rsp.vregs[r].u16[3]);
    case 0x0d:
        return _mm_set1_epi16(state->rsp.vregs[r].u16[2]);
    case 0x0e:
        return _mm_set1_epi16(state->rsp.vregs[r].u16[1]);
    case 0x0f:
        return _mm_set1_epi16(state->rsp.vregs[r].u16[0]);
    }
    __builtin_unreachable();
}

static uint32_t _runCycles(AzState* state, uint32_t cycles)
{
    uint32_t i;
    uint32_t op;
    uint16_t pc;
    uint16_t pc2;
    uint32_t tmp;
    uint32_t* regs;
    __m128i a;
    __m128i b;
    __m128i c;
    __m128i d;
    __m128i hi;
    __m128i lo;
    __m128i acc_hi;
    __m128i acc_md;
    __m128i acc_lo;

    regs = state->rsp.registers;
    pc = state->rsp.pc;
    pc2 = state->rsp.pc2;

    acc_hi = _mm_load_si128(&state->rsp.vacc_hi.vi);
    acc_md = _mm_load_si128(&state->rsp.vacc_md.vi);
    acc_lo = _mm_load_si128(&state->rsp.vacc_lo.vi);

    for (i = 0; i < cycles; ++i)
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
                printf("RSP BREAK PC: 0x%04x OP: 0x%08x\n", pc, op);
                state->rspWorker.enabled = 0;
                state->rsp.cregs[RSP_CREG_SP_STATUS] |= 0x03;
                if (state->rsp.cregs[RSP_CREG_SP_STATUS] & 0x40)
                    azRcpRaiseInterrupt(state, RCP_INTR_SP);
                goto end;
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
            default:
                TRAP;
                break;
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
                tmp = azRspControlRead(state, RD);
                if (RT) regs[RT] = tmp;
                break;
            case OP_COP_MT:
                azRspControlWrite(state, RD, regs[RT]);
                break;
            }
            break;
        case OP_COP2:
            switch (RS)
            {
            default:
                TRAP;
                break;
            case OP_COP_MT:
                state->rsp.vregs[RD].u16[7 - (VE / 2)] = (regs[RT] & 0xffff);
                break;
            case OP_COP_MF:
                regs[RT] = (int32_t)((int16_t)state->rsp.vregs[RD].u16[7 - (VE / 2)]);
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
                case OP_CP2_VMULF:
                    a = vLoad(state, VT);
                    b = vLoadE(state, VS, E);
                    hi = _mm_mulhi_epi16(a, b);
                    lo = _mm_mullo_epi16(a, b);
                    acc_hi = _mm_or_si128(_mm_slli_epi16(hi, 1), _mm_srli_epi16(lo, 15));
                    acc_md = _mm_slli_epi16(lo, 1);
                    c = _mm_srli_epi16(acc_lo, 15);
                    acc_lo = _mm_add_epi16(acc_lo, _mm_set1_epi16(0x8000));
                    d = _mm_srli_epi16(_mm_cmpeq_epi16(acc_md, _mm_set1_epi16(0xffff)), 15);
                    acc_md = _mm_add_epi16(acc_md, c);
                    acc_hi = _mm_add_epi16(acc_hi, d);
                    c = _mm_unpacklo_epi16(acc_md, acc_hi);
                    d = _mm_unpackhi_epi16(acc_md, acc_hi);
                    vStore(state, VD, _mm_packs_epi32(c, d));
                    break;
                case OP_CP2_VMULU:
                    a = vLoad(state, VT);
                    b = vLoadE(state, VS, E);
                    hi = _mm_mulhi_epu16(a, b);
                    lo = _mm_mullo_epi16(a, b);
                    acc_hi = _mm_or_si128(_mm_slli_epi16(hi, 1), _mm_srli_epi16(lo, 15));
                    acc_md = _mm_slli_epi16(lo, 1);
                    c = _mm_srli_epi16(acc_lo, 15);
                    acc_lo = _mm_add_epi16(acc_lo, _mm_set1_epi16(0x8000));
                    d = _mm_srli_epi16(_mm_cmpeq_epi16(acc_md, _mm_set1_epi16(0xffff)), 15);
                    acc_md = _mm_add_epi16(acc_md, c);
                    acc_hi = _mm_add_epi16(acc_hi, d);
                    c = _mm_xor_si128(_mm_cmpeq_epi16(acc_hi, _mm_setzero_si128()), _mm_setzero_si128());
                    vStore(state, VD, _mm_adds_epu16(acc_md, c));
                    break;
                case OP_CP2_VRNDP:
                    TRAP;
                    break;
                case OP_CP2_VMULQ:
                    TRAP;
                    break;
                case OP_CP2_VMUDL:
                    TRAP;
                    break;
                case OP_CP2_VMUDM:
                    TRAP;
                    break;
                case OP_CP2_VMUDN:
                    TRAP;
                    break;
                case OP_CP2_VMUDH:
                    TRAP;
                    break;
                case OP_CP2_VMACF:
                    a = vLoad(state, VT);
                    b = vLoadE(state, VS, E);
                    hi = _mm_mulhi_epi16(a, b);
                    lo = _mm_mullo_epi16(a, b);
                    hi = _mm_or_si128(_mm_slli_epi16(hi, 1), _mm_srli_epi16(lo, 15));
                    lo = _mm_slli_epi16(lo, 1);
                    c = _mm_srli_epi16(_mm_and_si128(acc_md, lo), 15);
                    acc_hi = _mm_add_epi16(_mm_add_epi16(acc_hi, hi), c);
                    acc_md = _mm_add_epi16(acc_md, lo);
                    c = _mm_unpacklo_epi16(acc_md, acc_hi);
                    d = _mm_unpackhi_epi16(acc_md, acc_hi);
                    vStore(state, VD, _mm_packs_epi32(c, d));
                    break;
                case OP_CP2_VMACU:
                    TRAP;
                    break;
                case OP_CP2_VRNDN:
                    TRAP;
                    break;
                case OP_CP2_VMACQ:
                    TRAP;
                    break;
                case OP_CP2_VMADL:
                    TRAP;
                    break;
                case OP_CP2_VMADM:
                    TRAP;
                    break;
                case OP_CP2_VMADN:
                    TRAP;
                    break;
                case OP_CP2_VMADH:
                    TRAP;
                    break;
                case OP_CP2_VADD:
                    TRAP;
                    break;
                case OP_CP2_VSUB:
                    TRAP;
                    break;
                case OP_CP2_VABS:
                    TRAP;
                    break;
                case OP_CP2_VADDC:
                    TRAP;
                    break;
                case OP_CP2_VSUBC:
                    TRAP;
                    break;
                case OP_CP2_VSAR:
                    TRAP;
                    break;
                case OP_CP2_VLT:
                    TRAP;
                    break;
                case OP_CP2_VEQ:
                    TRAP;
                    break;
                case OP_CP2_VNE:
                    TRAP;
                    break;
                case OP_CP2_VGE:
                    TRAP;
                    break;
                case OP_CP2_VCL:
                    TRAP;
                    break;
                case OP_CP2_VCH:
                    TRAP;
                    break;
                case OP_CP2_VCR:
                    TRAP;
                    break;
                case OP_CP2_VMGR:
                    TRAP;
                    break;
                case OP_CP2_VAND:
                    TRAP;
                    break;
                case OP_CP2_VNAND:
                    TRAP;
                    break;
                case OP_CP2_VOR:
                    TRAP;
                    break;
                case OP_CP2_VNOR:
                    TRAP;
                    break;
                case OP_CP2_VXOR:
                    TRAP;
                    break;
                case OP_CP2_VNXOR:
                    TRAP;
                    break;
                case OP_CP2_VRCP:
                    TRAP;
                    break;
                case OP_CP2_VRCPL:
                    TRAP;
                    break;
                case OP_CP2_VRCPH:
                    TRAP;
                    break;
                case OP_CP2_VMOV:
                    TRAP;
                    break;
                case OP_CP2_VRSQ:
                    TRAP;
                    break;
                case OP_CP2_VRSQL:
                    TRAP;
                    break;
                case OP_CP2_VRSQH:
                    TRAP;
                    break;
                case OP_CP2_VNOP:
                    TRAP;
                    break;
                }
                break;
            }
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
        case OP_LWC2:
            switch (RD)
            {
            default:
                TRAP;
                break;
            case OP_LWC2_LBV:
                state->rsp.vregs[RT].u8[15 - VE] = *(uint8_t*)(state->spDmem + (regs[RS] & 0xfff) + VOFF);
                break;
            case OP_LWC2_LSV:
                state->rsp.vregs[RT].u16[7 - (VE / 2)] = bswap16(*(uint16_t*)(state->spDmem + (regs[RS] & 0xfff) + (VOFF << 1)));
                break;
            case OP_LWC2_LLV:
                state->rsp.vregs[RT].u32[3 - (VE / 4)] = bswap32(*(uint32_t*)(state->spDmem + (regs[RS] & 0xfff) + (VOFF << 2)));
                break;
            case OP_LWC2_LDV:
                state->rsp.vregs[RT].u64[1 - (VE / 8)] = bswap64(*(uint64_t*)(state->spDmem + (regs[RS] & 0xfff) + (VOFF << 3)));
                break;
            case OP_LWC2_LQV:
                tmp = (regs[RS] + VOFF) & 0xfff;
                bswapLoad128(state->rsp.vregs[RT].u8, 0, state->spDmem + tmp, ((tmp - 1) % 16) + 1);
                break;
            case OP_LWC2_LRV:
                tmp = (regs[RS] + VOFF) & 0xfff;
                bswapLoad128(state->rsp.vregs[RT].u8, tmp % 16, state->spDmem + (tmp & 0xff0), tmp % 16);
                break;
            case OP_LWC2_LPV:
                TRAP;
                break;
            case OP_LWC2_LUV:
                TRAP;
                break;
            case OP_LWC2_LHV:
                TRAP;
                break;
            case OP_LWC2_LFV:
                TRAP;
                break;
            case OP_LWC2_LTV:
                TRAP;
                break;
            }
            break;
        case OP_SWC2:
            switch (RD)
            {
            default:
                TRAP;
                break;
            case OP_SWC2_SBV:
                *(uint8_t*)(state->spDmem + (regs[RS] & 0xfff) + VOFF) = state->rsp.vregs[RT].u8[15 - VE];
                break;
            case OP_SWC2_SSV:
                *(uint16_t*)(state->spDmem + (regs[RS] & 0xfff) + (VOFF << 1)) = bswap16(state->rsp.vregs[RT].u16[7 - (VE / 2)]);
                break;
            case OP_SWC2_SLV:
                *(uint32_t*)(state->spDmem + (regs[RS] & 0xfff) + (VOFF << 2)) = bswap32(state->rsp.vregs[RT].u32[3 - (VE / 4)]);
                break;
            case OP_SWC2_SDV:
                *(uint64_t*)(state->spDmem + (regs[RS] & 0xfff) + (VOFF << 3)) = bswap64(state->rsp.vregs[RT].u64[1 - (VE / 8)]);
                break;
            case OP_SWC2_SQV:
                tmp = (regs[RS] + VOFF) & 0xfff;
                bswapStore128(state->spDmem + tmp, 0, state->rsp.vregs[RT].u8, ((tmp - 1) % 16) + 1);
                break;
            case OP_SWC2_SRV:
                TRAP;
                break;
            case OP_SWC2_SPV:
                TRAP;
                break;
            case OP_SWC2_SUV:
                TRAP;
                break;
            case OP_SWC2_SHV:
                TRAP;
                break;
            case OP_SWC2_SFV:
                TRAP;
                break;
            case OP_SWC2_SWV:
                TRAP;
                break;
            case OP_SWC2_STV:
                TRAP;
                break;
            }
            break;
        }
    }

end:
    _mm_store_si128(&state->rsp.vacc_hi.vi, acc_hi);
    _mm_store_si128(&state->rsp.vacc_md.vi, acc_md);
    _mm_store_si128(&state->rsp.vacc_lo.vi, acc_lo);

    state->rsp.pc = pc;
    state->rsp.pc2 = pc2;

    return i;
}

static inline uint64_t _getTimeNano()
{
    return clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
}

void* azRspWorkerMain(void* s)
{
    AzState* state = (AzState*)s;

    static const uint32_t granularity = 1024;
    uint64_t timeLastReport;
    uint64_t timeBefore;
    uint64_t timeNow;
    uint64_t duration;
    uint64_t cycles;

    cycles = 0;
    duration = 0;
    timeLastReport = _getTimeNano();
    for (;;)
    {
        azWorkerBarrier(&state->rspWorker);
        timeBefore = _getTimeNano();
        cycles += _runCycles(state, granularity);
        timeNow = _getTimeNano();
        duration += timeNow - timeBefore;
        if ((timeNow - timeLastReport) >= 1000000000)
        {
            double instrPerSecond = (double)cycles / ((double)duration * 1e-9);
            printf("RSP Vi/s: %.2fM   (PC: 0x%04x)\n", instrPerSecond / 1000000, state->rsp.pc);
            duration = 0;
            cycles = 0;
            timeLastReport = timeNow;
        }
    }
}
