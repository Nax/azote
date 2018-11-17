#ifndef INCLUDED_AZOTE_LIBAZOTE_H
#define INCLUDED_AZOTE_LIBAZOTE_H

/* This file is the private libazote header */

#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <x86intrin.h>
#include <azote/azote.h>

uint32_t azCRC32(void* data, size_t len);

inline static uint8_t bswap8(uint8_t in)
{
    return in;
}

inline static uint16_t bswap16(uint16_t in)
{
    return ((in << 8) | (in >> 8));
}

inline static uint32_t bswap32(uint32_t in)
{
    uint32_t out = 0;

    out |= ((in << 24) & 0xff000000);
    out |= ((in <<  8) & 0x00ff0000);
    out |= ((in >>  8) & 0x0000ff00);
    out |= ((in >> 24) & 0x000000ff);

    return out;
}

inline static uint64_t bswap64(uint64_t in)
{
    uint64_t out = 0;

    out |= ((in << 56) & 0xff00000000000000ULL);
    out |= ((in << 40) & 0x00ff000000000000ULL);
    out |= ((in << 24) & 0x0000ff0000000000ULL);
    out |= ((in <<  8) & 0x000000ff00000000ULL);
    out |= ((in >>  8) & 0x00000000ff000000ULL);
    out |= ((in >> 24) & 0x0000000000ff0000ULL);
    out |= ((in >> 40) & 0x000000000000ff00ULL);
    out |= ((in >> 56) & 0x00000000000000ffULL);

    return out;
}

inline static uint64_t sext8(uint8_t v)
{
    return ((int64_t)((int8_t)v));
}

inline static uint64_t sext16(uint16_t v)
{
    return ((int64_t)((int16_t)v));
}

inline static uint64_t sext32(uint32_t v)
{
    return ((int64_t)((int32_t)v));
}

inline static uint32_t fpS2W(float f)
{
    uint32_t value = signbit(f) ? 0x80000000 : 0x0;
    value |= ((uint32_t)fabsf(f) & 0x7fffffff);
    return value;
}

inline static uint64_t fpS2L(float f)
{
    uint64_t value = signbit(f) ? 0x8000000000000000 : 0x0;
    value |= ((uint64_t)fabsf(f) & 0x7fffffffffffffff);
    return value;
}

inline static uint32_t fpD2W(double d)
{
    uint32_t value = signbit(d) ? 0x80000000 : 0x0;
    value |= ((uint32_t)fabs(d) & 0x7fffffff);
    return value;
}

inline static uint64_t fpD2L(double d)
{
    uint64_t value = signbit(d) ? 0x8000000000000000 : 0x0;
    value |= ((uint64_t)fabs(d) & 0x7fffffffffffffff);
    return value;
}

inline static float fpW2S(uint32_t w)
{
    float value = (float)(w & 0x7fffffff);
    if (w & 0x80000000)
        value = -value;
    return value;
}

inline static double fpW2D(uint32_t w)
{
    double value = (double)(w & 0x7fffffff);
    if (w & 0x80000000)
        value = -value;
    return value;
}

inline static float fpL2S(uint64_t l)
{
    float value = (float)(l & 0x7fffffffffffffff);
    if (l & 0x8000000000000000)
        value = -value;
    return value;
}

inline static double fpL2D(uint64_t l)
{
    double value = (double)(l & 0x7fffffffffffffff);
    if (l & 0x8000000000000000)
        value = -value;
    return value;
}

void azDebugDumpState(AzState* state);
void azDebugHexdumpRaw(char* addr, size_t len);

#define AZOTE_MEMORY_SIZE   (8 * 1024 * 1024)

uint32_t    azPhysicalAddress(AzState* state, uint64_t vaddr);

uint8_t     azMemoryRead8(AzState* state, uint64_t addr);
uint16_t    azMemoryRead16(AzState* state, uint64_t addr);
uint32_t    azMemoryRead32(AzState* state, uint64_t addr);
uint64_t    azMemoryRead64(AzState* state, uint64_t addr);

void azMemoryWrite8(AzState* state, uint64_t addr, uint8_t value);
void azMemoryWrite16(AzState* state, uint64_t addr, uint16_t value);
void azMemoryWrite32(AzState* state, uint64_t addr, uint32_t value);
void azMemoryWrite64(AzState* state, uint64_t addr, uint64_t value);

void azRunRSP(AzState* state);

#define CIC_UNKNOWN     0
#define CIC_6101        1
#define CIC_6102        2
#define CIC_6103        3
#define CIC_6105        4
#define CIC_6106        5

int azRomGetCIC(void* cart);

/* COP0 */
#define COP0_REG_INDEX          0
#define COP0_REG_RANDOM         1
#define COP0_REG_ENTRYLO0       2
#define COP0_REG_ENTRYLO1       3
#define COP0_REG_CONTEXT        4
#define COP0_REG_PAGE_MASK      5
#define COP0_REG_WIRED          6
#define COP0_REG_BADVADDR       8
#define COP0_REG_COUNT          9
#define COP0_REG_ENTRYHI        10
#define COP0_REG_COMPARE        11
#define COP0_REG_STATUS         12
#define COP0_REG_CAUSE          13
#define COP0_REG_EPC            14
#define COP0_REG_PRID           15
#define COP0_REG_CONFIG         16
#define COP0_REG_LLADDR         17
#define COP0_REG_WATCHLO        18
#define COP0_REG_WATCHHI        19
#define COP0_REG_XCONTEXT       20
#define COP0_REG_PARITY_ERROR   26
#define COP0_REG_CACHEERR       27
#define COP0_REG_TAGLO          28
#define COP0_REG_TAGHI          29
#define COP0_REG_ERROR_EPC      30

#define RSP_CREG_DMA_CACHE          0
#define RSP_CREG_DMA_DRAM           1
#define RSP_CREG_DMA_READ_LENGTH    2
#define RSP_CREG_DMA_WRITE_LENGTH   3
#define RSP_CREG_SP_STATUS          4
#define RSP_CREG_DMA_FULL           5
#define RSP_CREG_DMA_BUSY           6
#define RSP_CREG_SP_RESERVED        7
#define RSP_CREG_CMD_START          8
#define RSP_CREG_CMD_END            9
#define RSP_CREG_CMD_CURRENT        10
#define RSP_CREG_CMD_STATUS         11
#define RSP_CREG_CMD_CLOCK          12
#define RSP_CREG_CMD_BUSY           13
#define RSP_CREG_CMD_PIPE_BUSY      14
#define RSP_CREG_CMD_TMEM_BUSY      15

/* RCP */

#define RCP_INTR_SP 0
#define RCP_INTR_SI 1
#define RCP_INTR_AI 2
#define RCP_INTR_VI 3
#define RCP_INTR_PI 4
#define RCP_INTR_DP 5

#define MI_INIT_MODE_REG    0x04300000
#define MI_VERSION_REG      0x04300004
#define MI_INTR_REG         0x04300008
#define MI_INTR_MASK_REG    0x0430000c

#define MI_INTR_SP  0x01
#define MI_INTR_SI  0x02
#define MI_INTR_AI  0x04
#define MI_INTR_VI  0x08
#define MI_INTR_PI  0x10
#define MI_INTR_DP  0x20

#define SP_MEM_ADDR_REG     0x04040000
#define SP_DRAM_ADDR_REG    0x04040004
#define SP_RD_LEN_REG       0x04040008
#define SP_WR_LEN_REG       0x0404000c
#define SP_STATUS_REG       0x04040010
#define SP_DMA_FULL_REG     0x04040014
#define SP_DMA_BUSY_REG     0x04040018
#define SP_SEMAPHORE_REG    0x0404001c
#define SP_CMD_START        0x04100000
#define SP_CMD_END          0x04100004
#define SP_CMD_CURRENT      0x04100008
#define SP_CMD_STATUS       0x0410000c
#define SP_PC_REG           0x04080000
#define SP_IBIST_REG        0x04080004

#define VI_STATUS_REG   0x04400000
#define VI_ORIGIN_REG   0x04400004
#define VI_WIDTH_REG    0x04400008
#define VI_INTR_REG     0x0440000c
#define VI_CURRENT_REG  0x04400010
#define VI_BURST_REG    0x04400014
#define VI_V_SYNC_REG   0x04400018
#define VI_H_SYNC_REG   0x0440001c
#define VI_LEAP_REG     0x04400020
#define VI_H_START_REG  0x04400024
#define VI_V_START_REG  0x04400028
#define VI_V_BURST_REG  0x0440002c
#define VI_X_SCALE_REG  0x04400030
#define VI_Y_SCALE_REG  0x04400034

#define AI_DRAM_ADDR_REG    0x04500000
#define AI_LEN_REG          0x04500004
#define AI_CONTROL_REG      0x04500008
#define AI_STATUS_REG       0x0450000c
#define AI_DACRATE_REG      0x04500010
#define AI_BITRATE_REG      0x04500014

#define PI_DRAM_ADDR_REG        0x04600000
#define PI_CART_ADDR_REG        0x04600004
#define PI_RD_LEN_REG           0x04600008
#define PI_WR_LEN_REG           0x0460000c
#define PI_STATUS_REG           0x04600010
#define PI_BSD_DOM1_LAT_REG     0x04600014
#define PI_BSD_DOM1_PWD_REG     0x04600018
#define PI_BSD_DOM1_PGS_REG     0x0460001c
#define PI_BSD_DOM1_RLS_REG     0x04600020
#define PI_BSD_DOM2_LAT_REG     0x04600024
#define PI_BSD_DOM2_PWD_REG     0x04600028
#define PI_BSD_DOM2_PGS_REG     0x0460002c
#define PI_BSD_DOM2_RLS_REG     0x04600030

#define SI_DRAM_ADDR_REG        0x04800000
#define SI_PIF_ADDR_RD64B_REG   0x04800004
#define SI_PIF_ADDR_WR64B_REG   0x04800010
#define SI_STATUS_REG           0x04800018

uint32_t    azRspControlRead(AzState* state, uint8_t creg);
void        azRspControlWrite(AzState* state, uint8_t creg, uint32_t value);

void        azRcpClearInterrupt(AzState* state, uint8_t intr);
void        azRcpRaiseInterrupt(AzState* state, uint8_t intr);
void        azRcpCheckInterrupts(AzState* state);

uint32_t    azReadPIF(AzState* state, uint32_t addr);
uint32_t    azRcpReadSP(AzState* state, uint32_t addr);
uint32_t    azRcpReadMI(AzState* state, uint32_t addr);
uint32_t    azRcpReadPI(AzState* state, uint32_t addr);
uint32_t    azRcpReadAI(AzState* state, uint32_t addr);
uint32_t    azRcpReadVI(AzState* state, uint32_t addr);
uint32_t    azRcpReadSI(AzState* state, uint32_t addr);

void        azWritePIF(AzState* state, uint32_t addr, uint32_t value);
void        azRcpWriteSP(AzState* state, uint32_t addr, uint32_t value);
void        azRcpWriteMI(AzState* state, uint32_t addr, uint32_t value);
void        azRcpWritePI(AzState* state, uint32_t addr, uint32_t value);
void        azRcpWriteAI(AzState* state, uint32_t addr, uint32_t value);
void        azRcpWriteVI(AzState* state, uint32_t addr, uint32_t value);
void        azRcpWriteSI(AzState* state, uint32_t addr, uint32_t value);

void* azCpuWorkerMain(void*);
void* azRspWorkerMain(void*);

typedef _Atomic uint32_t atomic_u32;

typedef struct AzCOP0_  AzCOP0;
typedef struct AzCOP1_  AzCOP1;
typedef struct AzTLB_   AzTLB;

typedef union {
    uint64_t    u64;
    int64_t     i64;
    uint32_t    u32;
    int32_t     i32;
    float       f;
    double      d;
} AzReg;

typedef union {
    uint8_t     u8[16];
    uint16_t    u16[8];
    uint32_t    u32[4];
    uint64_t    u64[2];
    float       f[4];
    double      d[2];
    __m128i     vi;
} AzVReg;

typedef struct {
    uint64_t    registers[32];
    uint64_t    hi;
    uint64_t    lo;
    uint64_t    pc;
    uint64_t    pc2;
    uint8_t     ll:1;
} AzCPU;

typedef struct {
    uint16_t    pc;
    uint16_t    pc2;
    uint32_t    registers[32];
    atomic_u32  cregs[16];
    AzVReg      vregs[32];
    AzVReg      vacc_hi;
    AzVReg      vacc_md;
    AzVReg      vacc_lo;
} AzCoreRSP;

struct AzCOP0_ {
    uint64_t    registers[32];
};

struct AzCOP1_ {
    AzReg       registers[32];
    uint32_t    fcr0;
    uint32_t    fcr31;
};

struct AzTLB_ {
    uint64_t    lo0[32];
    uint64_t    lo1[32];
    uint64_t    hi[32];
    uint64_t    mask[32];
};

typedef struct {
    pthread_t       thread;
    _Atomic int     enabled;
    _Atomic int     enabledFeedback;
    pthread_cond_t  cond;
    pthread_cond_t  condFeedback;
    pthread_mutex_t mutex;
    pthread_mutex_t mutexFeedback;
} AzWorker;

struct AzState_ {
    AzWorker    cpuWorker;
    AzWorker    rspWorker;

    AzCPU       cpu;
    AzCoreRSP   rsp;
    AzCOP0      cop0;
    AzCOP1      cop1;
    AzTLB       tlb;
    uint64_t    cartSize;
    char*       cart;
    char*       rdram;
    char*       rdramRegisters;
    char*       spDmem;
    char*       spImem;
    uint32_t*   miRegisters;
    uint32_t*   viRegisters;
    uint32_t*   aiRegisters;
    uint32_t*   piRegisters;
    char*       riRegisters;
    uint32_t*   siRegisters;
    char*       pifram;
    unsigned    debug:1;
    unsigned    verbose:1;
};

int  azWorkerBarrier(AzWorker* worker);
void azWorkerStart(AzWorker* worker);
void azWorkerStop(AzWorker* worker);

#endif
