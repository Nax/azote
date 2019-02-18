#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <libazote/libazote.h>
#include <libazote/mips.h>

#define TRAP    do { printf("%s:%d - RSP TRAP at 0x%04x    OP: 0x%08x\n", __FILE__, __LINE__, pc, op); getchar(); } while (0)

static void* bswapStore128(void* RESTRICT dst, uint8_t base, void* RESTRICT src, size_t len)
{
    char* cDst = (char*)dst;
    char* cSrc = (char*)src;
    for (size_t i = 0; i < len; ++i)
        cDst[i] = cSrc[15 - base - i];
    return cDst;
}

static void bswap128(void* data)
{
    uint64_t* data64 = data;
    uint64_t tmp;

    tmp = bswap64(data64[0]);
    data64[0] = bswap64(data64[1]);
    data64[1] = tmp;
}

static __m128i vLoadE(__m128i* vregs, uint8_t r, uint8_t e)
{
    switch (e)
    {
    case 0x00:
        return vregs[r];
    case 0x02:
        return _mm_set1_epi64x(_mm_extract_epi64(vregs[r], 1));
    case 0x03:
        return _mm_set1_epi64x(_mm_extract_epi64(vregs[r], 0));
    case 0x04:
        return _mm_set1_epi32(_mm_extract_epi32(vregs[r], 3));
    case 0x05:
        return _mm_set1_epi32(_mm_extract_epi32(vregs[r], 2));
    case 0x06:
        return _mm_set1_epi32(_mm_extract_epi32(vregs[r], 1));
    case 0x07:
        return _mm_set1_epi32(_mm_extract_epi32(vregs[r], 0));
    case 0x08:
        return _mm_set1_epi16(_mm_extract_epi16(vregs[r], 7));
    case 0x09:
        return _mm_set1_epi16(_mm_extract_epi16(vregs[r], 6));
    case 0x0a:
        return _mm_set1_epi16(_mm_extract_epi16(vregs[r], 5));
    case 0x0b:
        return _mm_set1_epi16(_mm_extract_epi16(vregs[r], 4));
    case 0x0c:
        return _mm_set1_epi16(_mm_extract_epi16(vregs[r], 3));
    case 0x0d:
        return _mm_set1_epi16(_mm_extract_epi16(vregs[r], 2));
    case 0x0e:
        return _mm_set1_epi16(_mm_extract_epi16(vregs[r], 1));
    case 0x0f:
        return _mm_set1_epi16(_mm_extract_epi16(vregs[r], 0));
    }
    return _mm_setzero_si128();
}

static uint16_t vGetE(__m128i* vregs, uint8_t v, uint8_t e)
{
    AzVReg vtmp;

    _mm_store_si128(&vtmp.vi, vregs[v]);
    return vtmp.u16[7 - (e / 2)];
}

static void vSetE(__m128i* vregs, uint8_t v, uint8_t e, uint16_t value)
{
    __m128i a;
    __m128i mask;

    a = _mm_set1_epi16(value);
    mask = _mm_cmpeq_epi16(_mm_set_epi16(0, 2, 4, 6, 8, 10, 12, 14), _mm_cvtsi32_si128(e));
    vregs[v] = _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, vregs[v]));
}

static uint32_t _runCycles(AzState* state, uint32_t cycles)
{
    uint32_t i;
    uint32_t op;
    uint16_t pc;
    uint16_t pc2;
    uint32_t tmp;
    uint32_t tmp2;
    uint32_t regs[32];
    __m128i vregs[32];
    uint32_t div_in;
    uint32_t div_out;
    uint16_t vcc;
    uint16_t vco;
    uint8_t vce;
    __m128i a;
    __m128i b;
    __m128i c;
    __m128i d;
    __m128i hi;
    __m128i lo;
    __m128i carry;
    __m128i mask;
    __m128i acc_hi;
    __m128i acc_md;
    __m128i acc_lo;
    AzVReg vtmp;
    uint64_t tmp64;

    /* Copy regs */
    memcpy(regs, state->rsp.registers, sizeof(regs));

    /* Copy vector regs */
    for (size_t i = 0; i < 32; ++i)
        vregs[i] = _mm_load_si128(&state->rsp.vregs->vi);
    
    /* Copy other regs */
    div_in = state->rsp.div_in;
    div_out = state->rsp.div_out;
    vcc = state->rsp.vcc;
    vco = state->rsp.vco;
    vce = state->rsp.vce;

    /* Load PC */
    pc = state->rsp.pc;
    pc2 = state->rsp.pc2;

    /* Load acc */
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
            case OP_COP_CT:
                switch (RD)
                {
                case 0:
                    vco = regs[RT];
                    break;
                case 1:
                    vcc = regs[RT];
                    break;
                case 2:
                    vce = regs[RT];
                    break;
                default:
                    TRAP;
                    break;
                }
                break;
            case OP_COP_CF:
                if (RT)
                {
                    switch (RD)
                    {
                    case 0:
                        regs[RT] = vco;
                        break;
                    case 1:
                        regs[RT] = vcc;
                        break;
                    case 2:
                        regs[RT] = vce;
                        break;
                    default:
                        TRAP;
                        break;
                    }
                }
                break;
            case OP_COP_MT:
                a = _mm_set1_epi16(regs[RT] & 0xffff);
                mask = _mm_cmpeq_epi16(_mm_set_epi16(0, 2, 4, 6, 8, 10, 12, 14), _mm_cvtsi32_si128(VE));
                vregs[RD] = _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, vregs[RD]));
                break;
            case OP_COP_MF:
                _mm_store_si128(&vtmp.vi, vregs[RD]);
                regs[RT] = (int32_t)((int16_t)vtmp.u16[7 - (VE / 2)]);
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
                    a = vregs[VT];
                    b = vLoadE(vregs, VS, E);
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
                    vregs[VD] = _mm_packs_epi32(c, d);
                    break;
                case OP_CP2_VMULU:
                    a = vregs[VT];
                    b = vLoadE(vregs, VS, E);
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
                    vregs[VD] = _mm_adds_epu16(acc_md, c);
                    break;
                case OP_CP2_VRNDP:
                    TRAP;
                    break;
                case OP_CP2_VMULQ:
                    TRAP;
                    break;
                case OP_CP2_VMUDL:
                    a = vregs[VT];
                    b = vLoadE(vregs, VS, E);
                    hi = _mm_mulhi_epu16(a, b);
                    lo = _mm_mullo_epi16(a, b);
                    acc_md = _mm_cmplt_epi16(hi, _mm_setzero_si128());
                    acc_lo = hi;
                    /* TODO: Miss clamp */
                    vregs[VD] = lo;
                    break;
                case OP_CP2_VMUDM:
                    a = vregs[VT];
                    b = vLoadE(vregs, VS, E);
                    acc_md = _mm_mulhi_epi16(a, b);
                    acc_lo = _mm_mullo_epi16(a, b);
                    vregs[VD] = acc_md;
                    break;
                case OP_CP2_VMUDN:
                    a = vregs[VT];
                    b = vLoadE(vregs, VS, E);
                    acc_md = _mm_mulhi_epi16(a, b);
                    acc_lo = _mm_mullo_epi16(a, b);
                    vregs[VD] = acc_lo;
                    break;
                case OP_CP2_VMUDH:
                    a = vregs[VT];
                    b = vLoadE(vregs, VS, E);
                    acc_md = _mm_mulhi_epi16(a, b);
                    acc_lo = _mm_setzero_si128();
                    vregs[VD] = acc_md;
                    break;
                case OP_CP2_VMACF:
                    a = vregs[VT];
                    b = vLoadE(vregs, VS, E);
                    hi = _mm_mulhi_epi16(a, b);
                    lo = _mm_mullo_epi16(a, b);
                    hi = _mm_or_si128(_mm_slli_epi16(hi, 1), _mm_srli_epi16(lo, 15));
                    lo = _mm_slli_epi16(lo, 1);
                    c = _mm_srli_epi16(_mm_and_si128(acc_md, lo), 15);
                    acc_hi = _mm_add_epi16(_mm_add_epi16(acc_hi, hi), c);
                    acc_md = _mm_add_epi16(acc_md, lo);
                    c = _mm_unpacklo_epi16(acc_md, acc_hi);
                    d = _mm_unpackhi_epi16(acc_md, acc_hi);
                    vregs[VD] = _mm_packs_epi32(c, d);
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
                    a = vregs[VT];
                    b = vLoadE(vregs, VS, E);
                    hi = _mm_mulhi_epi16(a, b);
                    carry = _mm_srli_epi16(_mm_add_epi16(_mm_srli_epi16(hi, 15), _mm_srli_epi16(acc_lo, 15)), 1);
                    acc_md = _mm_add_epi16(acc_md, carry);
                    acc_lo = _mm_add_epi16(acc_lo, hi);
                    vregs[VD] = acc_lo;
                    break;
                case OP_CP2_VMADM:
                    a = vregs[VT];
                    b = vLoadE(vregs, VS, E);
                    hi = _mm_mulhi_epi16(a, b);
                    lo = _mm_mullo_epi16(a, b);
                    carry = _mm_srli_epi16(_mm_add_epi16(_mm_srli_epi16(lo, 15), _mm_srli_epi16(acc_lo, 15)), 1);
                    acc_md = _mm_add_epi16(_mm_add_epi16(acc_md, carry), hi);
                    acc_lo = _mm_add_epi16(acc_lo, lo);
                    vregs[VD] = acc_md;
                    break;
                case OP_CP2_VMADN:
                    a = vregs[VT];
                    b = vLoadE(vregs, VS, E);
                    hi = _mm_mulhi_epi16(a, b);
                    lo = _mm_mullo_epi16(a, b);
                    carry = _mm_srli_epi16(_mm_add_epi16(_mm_srli_epi16(lo, 15), _mm_srli_epi16(acc_lo, 15)), 1);
                    acc_md = _mm_add_epi16(_mm_add_epi16(acc_md, carry), hi);
                    acc_lo = _mm_add_epi16(acc_lo, lo);
                    vregs[VD] = acc_lo;
                    break;
                case OP_CP2_VMADH:
                    a = vregs[VT];
                    b = vLoadE(vregs, VS, E);
                    hi = _mm_mulhi_epi16(a, b);
                    acc_md = _mm_add_epi16(acc_md, hi);
                    vregs[VD] = acc_md;
                    break;
                case OP_CP2_VADD:
                    a = vregs[VT];
                    b = vLoadE(vregs, VS, E);
                    c = _mm_srli_epi16(_mm_cmpgt_epi16(_mm_and_si128(_mm_set1_epi16(vco), _mm_set_epi16(1, 2, 4, 8, 16, 32, 64, 128)), _mm_setzero_si128()), 15);
                    acc_lo = _mm_add_epi16(_mm_add_epi16(a, b), c);
                    vregs[VD] = acc_lo;
                    vco = 0;
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
                    /* DUMMY: Copy of VCH */
                    a = vregs[VS];
                    b = vLoadE(vregs, VT, E);
                    c = _mm_sub_epi16(_mm_setzero_si128(), b);
                    acc_lo = _mm_min_epi16(_mm_max_epi16(c, a), b);
                    vregs[VD] = acc_lo;

                    mask = _mm_cmplt_epi16(_mm_xor_si128(a, b), _mm_setzero_si128());

                    vce = 0;
                    vco = 0;
                    vcc = 0;

                    /* Start with VCE, it's 1 if a+b == 0xFFFF, 0 otherwise */
                    tmp = (uint32_t)_mm_movemask_epi8(_mm_cmpeq_epi16(_mm_add_epi16(a, b), _mm_set1_epi16(0xffff)));
                    vce |= ((tmp & 0x0001) >> 0);
                    vce |= ((tmp & 0x0004) >> 1);
                    vce |= ((tmp & 0x0010) >> 2);
                    vce |= ((tmp & 0x0040) >> 3);
                    vce |= ((tmp & 0x0100) >> 4);
                    vce |= ((tmp & 0x0400) >> 5);
                    vce |= ((tmp & 0x1000) >> 6);
                    vce |= ((tmp & 0x4000) >> 7);

                    /* Low 8 bits of VCO is sign equal */
                    tmp = (uint32_t)_mm_movemask_epi8(_mm_xor_si128(a, b));
                    vco |= ((tmp & 0x0002) >> 1);
                    vco |= ((tmp & 0x0008) >> 2);
                    vco |= ((tmp & 0x0020) >> 3);
                    vco |= ((tmp & 0x0080) >> 4);
                    vco |= ((tmp & 0x0200) >> 5);
                    vco |= ((tmp & 0x0800) >> 6);
                    vco |= ((tmp & 0x2000) >> 7);
                    vco |= ((tmp & 0x8000) >> 8);

                    /* High 8 bits of VCO is not equal */
                    tmp = ~((uint32_t)_mm_movemask_epi8(_mm_or_si128(_mm_cmpeq_epi16(a, b), _mm_cmpeq_epi16(a, c))));
                    vco |= ((tmp & 0x0001) << 8);
                    vco |= ((tmp & 0x0004) << 7);
                    vco |= ((tmp & 0x0010) << 6);
                    vco |= ((tmp & 0x0040) << 5);
                    vco |= ((tmp & 0x0100) << 4);
                    vco |= ((tmp & 0x0400) << 3);
                    vco |= ((tmp & 0x1000) << 2);
                    vco |= ((tmp & 0x4000) << 1);

                    /* LE */
                    c = _mm_cmpgt_epi16(_mm_setzero_si128(), _mm_add_epi16(a, b));
                    d = _mm_cmplt_epi16(b, _mm_setzero_si128());
                    tmp = ~((uint32_t)_mm_movemask_epi8(_mm_or_si128(_mm_and_si128(mask, c), _mm_andnot_si128(mask, d))));
                    vcc |= ((tmp & 0x0001) >> 0);
                    vcc |= ((tmp & 0x0004) >> 1);
                    vcc |= ((tmp & 0x0010) >> 2);
                    vcc |= ((tmp & 0x0040) >> 3);
                    vcc |= ((tmp & 0x0100) >> 4);
                    vcc |= ((tmp & 0x0400) >> 5);
                    vcc |= ((tmp & 0x1000) >> 6);
                    vcc |= ((tmp & 0x4000) >> 7);

                    /* GE */
                    c = _mm_cmplt_epi16(b, _mm_setzero_si128());
                    d = _mm_cmplt_epi16(_mm_setzero_si128(), _mm_sub_epi16(a, b));
                    tmp = ~((uint32_t)_mm_movemask_epi8(_mm_or_si128(_mm_and_si128(mask, c), _mm_andnot_si128(mask, d))));
                    vcc |= ((tmp & 0x0001) << 8);
                    vcc |= ((tmp & 0x0004) << 7);
                    vcc |= ((tmp & 0x0010) << 6);
                    vcc |= ((tmp & 0x0040) << 5);
                    vcc |= ((tmp & 0x0100) << 4);
                    vcc |= ((tmp & 0x0400) << 3);
                    vcc |= ((tmp & 0x1000) << 2);
                    vcc |= ((tmp & 0x4000) << 1);
                    break;
                case OP_CP2_VCH:
                    a = vregs[VS];
                    b = vLoadE(vregs, VT, E);
                    c = _mm_sub_epi16(_mm_setzero_si128(), b);
                    acc_lo = _mm_min_epi16(_mm_max_epi16(c, a), b);
                    vregs[VD] = acc_lo;

                    mask = _mm_cmplt_epi16(_mm_xor_si128(a, b), _mm_setzero_si128());

                    vce = 0;
                    vco = 0;
                    vcc = 0;

                    /* Start with VCE, it's 1 if a+b == 0xFFFF, 0 otherwise */
                    tmp = (uint32_t)_mm_movemask_epi8(_mm_cmpeq_epi16(_mm_add_epi16(a, b), _mm_set1_epi16(0xffff)));
                    vce |= ((tmp & 0x0001) >> 0);
                    vce |= ((tmp & 0x0004) >> 1);
                    vce |= ((tmp & 0x0010) >> 2);
                    vce |= ((tmp & 0x0040) >> 3);
                    vce |= ((tmp & 0x0100) >> 4);
                    vce |= ((tmp & 0x0400) >> 5);
                    vce |= ((tmp & 0x1000) >> 6);
                    vce |= ((tmp & 0x4000) >> 7);

                    /* Low 8 bits of VCO is sign equal */
                    tmp = (uint32_t)_mm_movemask_epi8(_mm_xor_si128(a, b));
                    vco |= ((tmp & 0x0002) >> 1);
                    vco |= ((tmp & 0x0008) >> 2);
                    vco |= ((tmp & 0x0020) >> 3);
                    vco |= ((tmp & 0x0080) >> 4);
                    vco |= ((tmp & 0x0200) >> 5);
                    vco |= ((tmp & 0x0800) >> 6);
                    vco |= ((tmp & 0x2000) >> 7);
                    vco |= ((tmp & 0x8000) >> 8);

                    /* High 8 bits of VCO is not equal */
                    tmp = ~((uint32_t)_mm_movemask_epi8(_mm_or_si128(_mm_cmpeq_epi16(a, b), _mm_cmpeq_epi16(a, c))));
                    vco |= ((tmp & 0x0001) << 8);
                    vco |= ((tmp & 0x0004) << 7);
                    vco |= ((tmp & 0x0010) << 6);
                    vco |= ((tmp & 0x0040) << 5);
                    vco |= ((tmp & 0x0100) << 4);
                    vco |= ((tmp & 0x0400) << 3);
                    vco |= ((tmp & 0x1000) << 2);
                    vco |= ((tmp & 0x4000) << 1);

                    /* LE */
                    c = _mm_cmpgt_epi16(_mm_setzero_si128(), _mm_add_epi16(a, b));
                    d = _mm_cmplt_epi16(b, _mm_setzero_si128());
                    tmp = ~((uint32_t)_mm_movemask_epi8(_mm_or_si128(_mm_and_si128(mask, c), _mm_andnot_si128(mask, d))));
                    vcc |= ((tmp & 0x0001) >> 0);
                    vcc |= ((tmp & 0x0004) >> 1);
                    vcc |= ((tmp & 0x0010) >> 2);
                    vcc |= ((tmp & 0x0040) >> 3);
                    vcc |= ((tmp & 0x0100) >> 4);
                    vcc |= ((tmp & 0x0400) >> 5);
                    vcc |= ((tmp & 0x1000) >> 6);
                    vcc |= ((tmp & 0x4000) >> 7);

                    /* GE */
                    c = _mm_cmplt_epi16(b, _mm_setzero_si128());
                    d = _mm_cmplt_epi16(_mm_setzero_si128(), _mm_sub_epi16(a, b));
                    tmp = ~((uint32_t)_mm_movemask_epi8(_mm_or_si128(_mm_and_si128(mask, c), _mm_andnot_si128(mask, d))));
                    vcc |= ((tmp & 0x0001) << 8);
                    vcc |= ((tmp & 0x0004) << 7);
                    vcc |= ((tmp & 0x0010) << 6);
                    vcc |= ((tmp & 0x0040) << 5);
                    vcc |= ((tmp & 0x0100) << 4);
                    vcc |= ((tmp & 0x0400) << 3);
                    vcc |= ((tmp & 0x1000) << 2);
                    vcc |= ((tmp & 0x4000) << 1);
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
                    tmp = vGetE(vregs, VT, E);
                    div_in = tmp << 16;
                    acc_lo = _mm_set1_epi16(tmp);
                    vSetE(vregs, VD, DE, (div_out >> 16));
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
                tmp64 = *(uint8_t*)(state->spDmem + (regs[RS] & 0xfff) + VOFF);
                a = _mm_set1_epi8(tmp64 & 0xff);
                mask = _mm_cmpeq_epi8(_mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), _mm_cvtsi32_si128(VE));
                vregs[RT] = _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, vregs[RT]));
                break;
            case OP_LWC2_LSV:
                tmp64 = bswap16(*(uint16_t*)(state->spDmem + (regs[RS] & 0xfff) + (VOFF << 1)));
                a = _mm_set1_epi16(tmp64 & 0xffff);
                mask = _mm_cmpeq_epi16(_mm_set_epi16(0, 2, 4, 6, 8, 10, 12, 14), _mm_cvtsi32_si128(VE));
                vregs[RT] = _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, vregs[RT]));
                break;
            case OP_LWC2_LLV:
                tmp64 = bswap32(*(uint32_t*)(state->spDmem + (regs[RS] & 0xfff) + (VOFF << 2)));
                a = _mm_set1_epi32(tmp64 & 0xffffffff);
                mask = _mm_cmpeq_epi32(_mm_set_epi32(0, 4, 8, 12), _mm_cvtsi32_si128(VE));
                vregs[RT] = _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, vregs[RT]));
                break;
            case OP_LWC2_LDV:
                tmp64 = bswap64(*(uint64_t*)(state->spDmem + (regs[RS] & 0xfff) + (VOFF << 3)));
                a = _mm_set1_epi64x(tmp64);
                mask = _mm_cmpeq_epi64(_mm_set_epi64x(0, 8), _mm_cvtsi32_si128(VE));
                vregs[RT] = _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, vregs[RT]));
                break;
            case OP_LWC2_LQV:
                tmp = (regs[RS] + (VOFF << 4)) & 0xfff;
                memcpy(&vtmp.u8, state->spDmem + (tmp & 0xff0), 16);
                bswap128(&vtmp.u8);
                a = _mm_load_si128(&vtmp.vi);
                mask = _mm_cmpgt_epi16(_mm_cvtsi32_si128(16 - (tmp & 0xf)), _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
                vregs[RT] = _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, vregs[RT]));
                break;
            case OP_LWC2_LRV:
                tmp = (regs[RS] + (VOFF << 4)) & 0xfff;
                memcpy(&vtmp.u8, state->spDmem + ((tmp - 16) & 0xff0), 16);
                bswap128(&vtmp.u8);
                a = _mm_load_si128(&vtmp.vi);
                mask = _mm_cmpgt_epi16(_mm_cvtsi32_si128(tmp & 0xf), _mm_setr_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
                vregs[RT] = _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, vregs[RT]));
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
    /* Store regs */
    memcpy(state->rsp.registers, regs, sizeof(regs));

    /* Store vector regs */
    for (size_t i = 0; i < 32; ++i)
        _mm_store_si128(&state->rsp.vregs->vi, vregs[i]);

    /* Store acc */
    _mm_store_si128(&state->rsp.vacc_hi.vi, acc_hi);
    _mm_store_si128(&state->rsp.vacc_md.vi, acc_md);
    _mm_store_si128(&state->rsp.vacc_lo.vi, acc_lo);

    /* Store other regs */
    state->rsp.div_in = div_in;
    state->rsp.div_out = div_out;
    state->rsp.vcc = vcc;
    state->rsp.vco = vco;
    state->rsp.vce = vce;

    /* Store PC */
    state->rsp.pc = pc;
    state->rsp.pc2 = pc2;

    return i;
}

void azRspWorkerMain(void* s)
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
    timeLastReport = azGetTimeNano();
    for (;;)
    {
        azWorkerBarrier(&state->rspWorker);
        timeBefore = azGetTimeNano();
        cycles += _runCycles(state, granularity);
        timeNow = azGetTimeNano();
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
