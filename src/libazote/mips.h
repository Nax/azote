#ifndef MIPS_H
#define MIPS_H

#include <stdint.h>

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
#define OP_LWC2     0x32
#define OP_LLD      0x34
#define OP_LDC1     0x35
#define OP_LD       0x37
#define OP_SC       0x38
#define OP_SWC1     0x39
#define OP_SWC2     0x3a
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

#define OP_COP1_S           0x10
#define OP_COP1_D           0x11
#define OP_COP1_W           0x14
#define OP_COP1_L           0x15

#define OP_CP1_ADD          0x00
#define OP_CP1_SUB          0x01
#define OP_CP1_MUL          0x02
#define OP_CP1_DIV          0x03
#define OP_CP1_SQRT         0x04
#define OP_CP1_ABS          0x05
#define OP_CP1_MOV          0x06
#define OP_CP1_NEG          0x07
#define OP_CP1_ROUND_L      0x08
#define OP_CP1_TRUNC_L      0x09
#define OP_CP1_CEIL_L       0x0a
#define OP_CP1_FLOOR_L      0x0b
#define OP_CP1_ROUND_W      0x0c
#define OP_CP1_TRUNC_W      0x0d
#define OP_CP1_CEIL_W       0x0e
#define OP_CP1_FLOOR_W      0x0f
#define OP_CP1_CVT_S        0x20
#define OP_CP1_CVT_D        0x21
#define OP_CP1_CVT_W        0x24
#define OP_CP1_CVT_L        0x25
#define OP_CP1_C_F          0x30
#define OP_CP1_C_UN         0x31
#define OP_CP1_C_EQ         0x32
#define OP_CP1_C_UEQ        0x33
#define OP_CP1_C_OLT        0x34
#define OP_CP1_C_ULT        0x35
#define OP_CP1_C_OLE        0x36
#define OP_CP1_C_ULE        0x37
#define OP_CP1_C_SF         0x38
#define OP_CP1_C_NGLE       0x39
#define OP_CP1_C_SEQ        0x3a
#define OP_CP1_C_NGL        0x3b
#define OP_CP1_C_LT         0x3c
#define OP_CP1_C_NGE        0x3d
#define OP_CP1_C_LE         0x3e
#define OP_CP1_C_NGT        0x3f

#define OP_LWC2_LBV         0x00
#define OP_LWC2_LSV         0x01
#define OP_LWC2_LLV         0x02
#define OP_LWC2_LDV         0x03
#define OP_LWC2_LQV         0x04
#define OP_LWC2_LRV         0x05
#define OP_LWC2_LPV         0x06
#define OP_LWC2_LUV         0x07
#define OP_LWC2_LHV         0x08
#define OP_LWC2_LFV         0x09
#define OP_LWC2_LTV         0x0b

#define OP_SWC2_SBV         0x00
#define OP_SWC2_SSV         0x01
#define OP_SWC2_SLV         0x02
#define OP_SWC2_SDV         0x03
#define OP_SWC2_SQV         0x04
#define OP_SWC2_SRV         0x05
#define OP_SWC2_SPV         0x06
#define OP_SWC2_SUV         0x07
#define OP_SWC2_SHV         0x08
#define OP_SWC2_SFV         0x09
#define OP_SWC2_SWV         0x0a
#define OP_SWC2_STV         0x0b

#define OP_CP2_VMULF        0x00
#define OP_CP2_VMULU        0x01
#define OP_CP2_VRNDP        0x02
#define OP_CP2_VMULQ        0x03
#define OP_CP2_VMUDL        0x04
#define OP_CP2_VMUDM        0x05
#define OP_CP2_VMUDN        0x06
#define OP_CP2_VMUDH        0x07
#define OP_CP2_VMACF        0x08
#define OP_CP2_VMACU        0x09
#define OP_CP2_VRNDN        0x0a
#define OP_CP2_VMACQ        0x0b
#define OP_CP2_VMADL        0x0c
#define OP_CP2_VMADM        0x0d
#define OP_CP2_VMADN        0x0e
#define OP_CP2_VMADH        0x0f
#define OP_CP2_VADD         0x10
#define OP_CP2_VSUB         0x11
#define OP_CP2_VABS         0x13
#define OP_CP2_VADDC        0x14
#define OP_CP2_VSUBC        0x15
#define OP_CP2_VSAR         0x1d
#define OP_CP2_VLT          0x20
#define OP_CP2_VEQ          0x21
#define OP_CP2_VNE          0x22
#define OP_CP2_VGE          0x23
#define OP_CP2_VCL          0x24
#define OP_CP2_VCH          0x25
#define OP_CP2_VCR          0x26
#define OP_CP2_VMGR         0x27
#define OP_CP2_VAND         0x28
#define OP_CP2_VNAND        0x29
#define OP_CP2_VOR          0x2a
#define OP_CP2_VNOR         0x2b
#define OP_CP2_VXOR         0x2c
#define OP_CP2_VNXOR        0x2d
#define OP_CP2_VRCP         0x30
#define OP_CP2_VRCPL        0x31
#define OP_CP2_VRCPH        0x32
#define OP_CP2_VMOV         0x33
#define OP_CP2_VRSQ         0x34
#define OP_CP2_VRSQL        0x35
#define OP_CP2_VRSQH        0x36
#define OP_CP2_VNOP         0x37

#define RS      ((op >> 21) & 0x1f)
#define RT      ((op >> 16) & 0x1f)
#define RD      ((op >> 11) & 0x1f)
#define SA      ((op >>  6) & 0x1f)

#define FT      RT
#define FS      RD
#define FD      SA

#define VT      RT
#define VS      RD
#define VD      SA
#define VE      ((op >> 7) & 0xf)
#define VOFF    (op & 0x7f)
#define E       ((op >> 21) & 0xf)

#define IMM     ((uint64_t)(op & 0xffff))
#define SIMM    ((int64_t)((int16_t)(op & 0xffff)))

static inline int32_t sra32(int32_t x, uint8_t shift)
{
    return (int32_t)(((uint32_t)x >> shift) | -(((uint32_t)x & ~(UINT32_MAX >> 1)) >> shift));
}

static inline int64_t sra64(int64_t x, uint8_t shift)
{
    return (int64_t)(((uint64_t)x >> shift) | -(((uint64_t)x & ~(UINT64_MAX >> 1)) >> shift));
}

#endif
