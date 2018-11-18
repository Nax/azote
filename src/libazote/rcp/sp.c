#include <stdio.h>
#include <string.h>
#include <libazote/libazote.h>

static void _dmaRDP(AzState* state)
{
    uint32_t index;
    size_t newCapacity;
    int fast;
    uint32_t size;
    uint32_t srcAddr;
    char* srcPtr;
    char* dst;

    puts("\n\n\n\n\nRDP DMA\n\n\n\n\n\n");
    getchar();
    if (state->rsp.cregs[RSP_CREG_CMD_END] < state->rsp.cregs[RSP_CREG_CMD_START])
        return;

    size = state->rsp.cregs[RSP_CREG_CMD_END] - state->rsp.cregs[RSP_CREG_CMD_START];
    if (size)
    {
        /* Check for XBus or fast RDRAM copy */
        if (state->rsp.cregs[RSP_CREG_CMD_STATUS] & 0x01)
        {
            srcPtr = state->spDmem + (state->rsp.cregs[RSP_CREG_CMD_START] & 0xfff);
            fast = 1;
        }
        else if (state->rsp.cregs[RSP_CREG_CMD_START] < AZOTE_MEMORY_SIZE)
        {
            srcPtr = state->rdram + state->rsp.cregs[RSP_CREG_CMD_START];
            fast = 1;
        }
        else
        {
            srcAddr = state->rsp.cregs[RSP_CREG_CMD_START];

            fast = 0;
        }

        /* Check for buffer space */
        index = 1 - state->rdpCommandBuffer.readIndex;
        pthread_mutex_lock(state->rdpCommandBuffer.mutex + index);
        while (state->rdpCommandBuffer.size[index] + size > state->rdpCommandBuffer.capacity[index])
        {
            newCapacity = state->rdpCommandBuffer.capacity[index] * 2;
            state->rdpCommandBuffer.data[index] = realloc(state->rdpCommandBuffer.data[index], newCapacity);
            state->rdpCommandBuffer.capacity[index] = newCapacity;
        }
        dst = state->rdpCommandBuffer.data[index] + state->rdpCommandBuffer.size[index];

        /* Copy */
        if (fast)
            memcpy(dst, srcPtr, size);
        else
        {
            for (size_t i = 0; i < size; ++i)
                *(uint8_t*)(dst + i) = azMemoryRead8(state, srcAddr + i);
        }
        state->rdpCommandBuffer.size[index] += size;
        state->rsp.cregs[RSP_CREG_CMD_CURRENT] = state->rsp.cregs[RSP_CREG_CMD_END];
        pthread_mutex_unlock(state->rdpCommandBuffer.mutex + index);
        azWorkerStart(&state->rdpWorker);
        puts("\n\n\n\n\nRDP DMA END\n\n\n\n\n\n");
        getchar();
    }
    else
    {
        state->rsp.cregs[RSP_CREG_CMD_CURRENT] = state->rsp.cregs[RSP_CREG_CMD_END];
    }
}

static void _dmaRead(AzState* state)
{
    char* src;
    char* dst;
    uint32_t pack;
    uint32_t length;
    uint32_t count;
    uint32_t skip;
    uint32_t ramAddr;
    uint32_t spAddr;

    puts("SP DMA (Read)");
    pack = state->rsp.cregs[RSP_CREG_DMA_READ_LENGTH];
    length = ((pack & 0xfff) | 0x7) + 1;
    count = ((pack >> 12) & 0xff) + 1;
    skip = (pack >> 20) & 0xfff;

    spAddr = state->rsp.cregs[RSP_CREG_DMA_CACHE] & 0x1fff;
    ramAddr = state->rsp.cregs[RSP_CREG_DMA_DRAM] & 0xffffff;

    if (spAddr & 0x1000)
        dst = state->spImem + (spAddr & 0xfff);
    else
        dst = state->spDmem + spAddr;
    src = state->rdram + ramAddr;

    if (skip == 0)
        memcpy(dst, src, length * count);
    else
    {
        for (size_t i = 0; i < count; ++i)
            memcpy(dst + i * length, src + i * (length + skip), length);
    }
    //azRcpRaiseInterrupt(state, RCP_INTR_SP);
    printf("0x%08x <- 0x%08x (Length: %u, Count: %u, Skip: %u)\n", spAddr, ramAddr, length, count, skip);
}

static void _dmaWrite(AzState* state)
{
    char* src;
    char* dst;
    uint32_t pack;
    uint32_t length;
    uint32_t count;
    uint32_t skip;
    uint32_t ramAddr;
    uint32_t spAddr;

    puts("SP DMA (Write)");
    pack = state->rsp.cregs[RSP_CREG_DMA_WRITE_LENGTH];
    length = ((pack & 0xfff) | 0x7) + 1;
    count = ((pack >> 12) & 0xff) + 1;
    skip = (pack >> 20) & 0xfff;

    spAddr = state->rsp.cregs[RSP_CREG_DMA_CACHE] & 0x1fff;
    ramAddr = state->rsp.cregs[RSP_CREG_DMA_DRAM] & 0xffffff;

    if (spAddr & 0x1000)
        src = state->spImem + (spAddr & 0xfff);
    else
        src = state->spDmem + spAddr;
    dst = state->rdram + ramAddr;

    if (skip == 0)
        memcpy(dst, src, length * count);
    else
    {
        for (size_t i = 0; i < count; ++i)
            memcpy(dst + i * length, src + i * (length + skip), length);
    }
    //azRcpRaiseInterrupt(state, RCP_INTR_SP);
    printf("0x%08x -> 0x%08x (Length: %u, Count: %u, Skip: %u)\n", spAddr, ramAddr, length, count, skip);
}

uint32_t azRspControlRead(AzState* state, uint8_t creg)
{
    printf("SP Read: 0x%02x\n", creg);
    switch (creg & 0x0f)
    {
    case RSP_CREG_DMA_CACHE:
        return state->rsp.cregs[creg];
    case RSP_CREG_DMA_DRAM:
        return state->rsp.cregs[creg];
    case RSP_CREG_DMA_READ_LENGTH:
        return state->rsp.cregs[creg];
    case RSP_CREG_DMA_WRITE_LENGTH:
        return state->rsp.cregs[creg];
    case RSP_CREG_SP_STATUS:
        return state->rsp.cregs[creg];
    case RSP_CREG_DMA_FULL:
        return state->rsp.cregs[creg];
    case RSP_CREG_DMA_BUSY:
        return state->rsp.cregs[creg];
    case RSP_CREG_SP_RESERVED:
        if (state->rsp.cregs[RSP_CREG_SP_RESERVED] == 0)
        {
            state->rsp.cregs[RSP_CREG_SP_RESERVED] = 1;
            return 0;
        }
        return 1;
    case RSP_CREG_CMD_START:
        return state->rsp.cregs[creg];
    case RSP_CREG_CMD_END:
        return state->rsp.cregs[creg];
    case RSP_CREG_CMD_CURRENT:
        return state->rsp.cregs[creg];
    case RSP_CREG_CMD_STATUS:
        return state->rsp.cregs[creg];
    case RSP_CREG_CMD_CLOCK:
        return state->rsp.cregs[creg];
    case RSP_CREG_CMD_BUSY:
        return state->rsp.cregs[creg];
    case RSP_CREG_CMD_PIPE_BUSY:
        return state->rsp.cregs[creg];
    case RSP_CREG_CMD_TMEM_BUSY:
        return state->rsp.cregs[creg];
    }
    return 0;
}

void azRspControlWrite(AzState* state, uint8_t creg, uint32_t value)
{
    uint32_t tmp;
    uint32_t status;

    printf("SP Write: 0x%02x 0x%08x\n", creg, value);
    switch (creg & 0x0f)
    {
    case RSP_CREG_DMA_CACHE:
        state->rsp.cregs[RSP_CREG_DMA_CACHE] = value;
        break;
    case RSP_CREG_DMA_DRAM:
        state->rsp.cregs[RSP_CREG_DMA_DRAM] = value;
        break;
    case RSP_CREG_DMA_READ_LENGTH:
        state->rsp.cregs[RSP_CREG_DMA_READ_LENGTH] = value;
        _dmaRead(state);
        break;
    case RSP_CREG_DMA_WRITE_LENGTH:
        state->rsp.cregs[RSP_CREG_DMA_WRITE_LENGTH] = value;
        _dmaWrite(state);
        break;
    case RSP_CREG_SP_STATUS:
        tmp = state->rsp.cregs[RSP_CREG_SP_STATUS];
        status = tmp;

        if (value & 0x00000001) status &= ~(0x0001);              // clear halt
        else if (value & 0x00000002) status |= 0x0001;            // set halt
        if (value & 0x00000004) status &= ~(0x0002);              // clear broke
        if (value & 0x00000008) azRcpClearInterrupt(state, RCP_INTR_SP);                        // clear intr
        else if (value & 0x00000010) azRcpRaiseInterrupt(state, RCP_INTR_SP);                   // set intr
        if (value & 0x00000020) status &= ~(0x0020);              // clear sstep
        else if (value & 0x00000040) status |= 0x0020;            // set sstep
        if (value & 0x00000080) status &= ~(0x0040);              // clear intr on break
        else if (value & 0x00000100) status |= 0x0040;            // set intr on break
        if (value & 0x00000200) status &= ~(0x0080);              // clear signal 0
        else if (value & 0x00000400) status |= 0x0080;            // set signal 0
        if (value & 0x00000800) status &= ~(0x0100);              // clear signal 1
        else if (value & 0x00001000) status |= 0x0100;            // set signal 1
        if (value & 0x00002000) status &= ~(0x0200);              // clear signal 2
        else if (value & 0x00004000) status |= 0x0200;            // set signal 2
        if (value & 0x00008000) status &= ~(0x0400);              // clear signal 3
        else if (value & 0x00010000) status |= 0x0400;            // set signal 3
        if (value & 0x00020000) status &= ~(0x0800);              // clear signal 4
        else if (value & 0x00040000) status |= 0x0800;            // set signal 4
        if (value & 0x00080000) status &= ~(0x1000);              // clear signal 5
        else if (value & 0x00100000) status |= 0x1000;            // set signal 5
        if (value & 0x002000000) status &= ~(0x2000);             // clear signal 6
        else if (value & 0x00400000) status |= 0x2000;            // set signal 6
        if (value & 0x00800000) status &= ~(0x4000);              // clear signal 7
        else if (value & 0x01000000) status |= 0x4000;            // set signal 7
        state->rsp.cregs[RSP_CREG_SP_STATUS] = status;

        if ((status & 0x01) != (tmp & 0x01))
        {
            //printf("RSP: %s\n", (state->rsp.cregs[RSP_CREG_SP_STATUS] & 0x01) ? "HALT" : "RUN");
            if (!(state->rsp.cregs[RSP_CREG_SP_STATUS] & 0x01))
            {
                azWorkerStart(&state->rspWorker);
            }
        }
        break;
    case RSP_CREG_DMA_FULL:
        break;
    case RSP_CREG_DMA_BUSY:
        break;
    case RSP_CREG_SP_RESERVED:
        if (value == 0)
            state->rsp.cregs[RSP_CREG_SP_RESERVED] = 0;
        break;
    case RSP_CREG_CMD_START:
        state->rsp.cregs[RSP_CREG_CMD_START] = value;
        break;
    case RSP_CREG_CMD_END:
        state->rsp.cregs[RSP_CREG_CMD_END] = value;
        _dmaRDP(state);
        break;
    case RSP_CREG_CMD_CURRENT:
        break;
    case RSP_CREG_CMD_STATUS:
        status = state->rsp.cregs[RSP_CREG_CMD_STATUS];
        if (value & 0x0001) status &= ~0x0001;
        else if (value & 0x0002) status |= 0x0001;
        if (value & 0x0004) status &= ~0x0002;
        else if (value & 0x0008) status |= 0x0002;
        if (value & 0x0010) status &= ~0x0004;
        else if (value & 0x0020) status |= 0x0004;
        state->rsp.cregs[RSP_CREG_CMD_STATUS] = status;
        break;
    case RSP_CREG_CMD_CLOCK:
        break;
    case RSP_CREG_CMD_BUSY:
        break;
    case RSP_CREG_CMD_PIPE_BUSY:
        break;
    case RSP_CREG_CMD_TMEM_BUSY:
        break;
    }
}

uint32_t azRcpReadSP(AzState* state, uint32_t addr)
{
    switch (addr)
    {
    case SP_MEM_ADDR_REG:
        return azRspControlRead(state, RSP_CREG_DMA_CACHE);
    case SP_DRAM_ADDR_REG:
        return azRspControlRead(state, RSP_CREG_DMA_DRAM);
    case SP_RD_LEN_REG:
        return azRspControlRead(state, RSP_CREG_DMA_READ_LENGTH);
    case SP_WR_LEN_REG:
        return azRspControlRead(state, RSP_CREG_DMA_WRITE_LENGTH);
    case SP_STATUS_REG:
        return azRspControlRead(state, RSP_CREG_SP_STATUS);
    case SP_CMD_START:
        return azRspControlRead(state, RSP_CREG_CMD_START);
    case SP_CMD_END:
        return azRspControlRead(state, RSP_CREG_CMD_END);
    case SP_CMD_CURRENT:
        return azRspControlRead(state, RSP_CREG_CMD_CURRENT);
    case SP_CMD_STATUS:
        return azRspControlRead(state, RSP_CREG_CMD_STATUS);
    case SP_CMD_CLOCK:
        return azRspControlRead(state, RSP_CREG_CMD_CLOCK);
    case SP_CMD_BUSY:
        return azRspControlRead(state, RSP_CREG_CMD_BUSY);
    case SP_CMD_PIPE_BUSY:
        return azRspControlRead(state, RSP_CREG_CMD_PIPE_BUSY);
    case SP_CMD_TMEM_BUSY:
        return azRspControlRead(state, RSP_CREG_CMD_TMEM_BUSY);
    case SP_PC_REG:
        return state->rsp.pc & 0xfff;
    default:
        return 0;
    }
}

void azRcpWriteSP(AzState* state, uint32_t addr, uint32_t value)
{
    switch (addr)
    {
    case SP_MEM_ADDR_REG:
        azRspControlWrite(state, RSP_CREG_DMA_CACHE, value);
        return;
    case SP_DRAM_ADDR_REG:
        azRspControlWrite(state, RSP_CREG_DMA_DRAM, value);
        return;
    case SP_RD_LEN_REG:
        azRspControlWrite(state, RSP_CREG_DMA_READ_LENGTH, value);
        return;
    case SP_WR_LEN_REG:
        azRspControlWrite(state, RSP_CREG_DMA_WRITE_LENGTH, value);
        return;
    case SP_STATUS_REG:
        azRspControlWrite(state, RSP_CREG_SP_STATUS, value);
        return;
    case SP_CMD_START:
        azRspControlWrite(state, RSP_CREG_CMD_START, value);
        return;
    case SP_CMD_END:
        azRspControlWrite(state, RSP_CREG_CMD_END, value);
        return;
    case SP_CMD_CURRENT:
        azRspControlWrite(state, RSP_CREG_CMD_CURRENT, value);
        return;
    case SP_CMD_STATUS:
        azRspControlWrite(state, RSP_CREG_CMD_STATUS, value);
        return;
    case SP_CMD_CLOCK:
        azRspControlWrite(state, RSP_CREG_CMD_CLOCK, value);
        return;
    case SP_CMD_BUSY:
        azRspControlWrite(state, RSP_CREG_CMD_BUSY, value);
        return;
    case SP_CMD_PIPE_BUSY:
        azRspControlWrite(state, RSP_CREG_CMD_PIPE_BUSY, value);
        return;
    case SP_CMD_TMEM_BUSY:
        azRspControlWrite(state, RSP_CREG_CMD_TMEM_BUSY, value);
        return;
    case SP_PC_REG:
        state->rsp.pc = value & 0xfff;
        state->rsp.pc2 = state->rsp.pc + 4;
        return;
    default:
        return;
    }
}
