#include <stdio.h>
#include <libazote/libazote.h>

#define TRAP  do { printf("Unknown RDP command, INSTR: 0x%016llx  OP: 0x%02x\n", instr, op); /*getchar();*/ } while(0)
#define TRAP

#define OP_NO_OP                    0x00
#define OP_TEXTURE_RECTANGLE        0x24
#define OP_TEXTURE_RECTANGLE_FLIP   0x25
#define OP_SYNC_PIPE                0x27
#define OP_SYNC_TILE                0x28
#define OP_SYNC_FULL                0x29
#define OP_SET_KEY_GB               0x2a
#define OP_SET_KEY_R                0x2b
#define OP_SET_CONVERT              0x2c
#define OP_SET_SCISSOR              0x2d
#define OP_SET_PRIM_DEPTH           0x2e
#define OP_SET_OTHER_MODES          0x2f
#define OP_LOAD_TLUT                0x30
#define OP_SYNC_LOAD                0x31
#define OP_SET_TILE_SIZE            0x32
#define OP_LOAD_BLOCK               0x33
#define OP_LOAD_TILE                0x34
#define OP_SET_TILE                 0x35
#define OP_FILL_RECTANGLE           0x36
#define OP_SET_FILL_COLOR           0x37
#define OP_SET_FOG_COLOR            0x38
#define OP_SET_BLEND_COLOR          0x39
#define OP_SET_PRIM_COLOR           0x3a
#define OP_SET_ENV_COLOR            0x3b
#define OP_SET_COMBINE_MODE         0x3c
#define OP_SET_TEXTURE_IMAGE        0x3d
#define OP_SET_Z_IMAGE              0x3e
#define OP_SET_COLOR_IMAGE          0x3f

static float fixed10_2(uint16_t fixed)
{
    // TODO: use some bits hack
    float f = (fixed >> 2);
    f += 0.25 * (fixed & 0x3);
    return f;
}

static void runCommandBuffer(AzState* state, char* buffer, size_t length)
{
    uint64_t    instr;
    uint8_t     op;
    float       fa;
    float       fb;
    float       fc;
    float       fd;

    for (size_t i = 0; i < length / 8; ++i)
    {
        instr = bswap64(*(uint64_t*)(buffer + i * 8));
        op = (instr >> 56) & 0x3f;
        switch (op)
        {
        default:
            TRAP;
            break;
        case OP_NO_OP:
            TRAP;
            break;
        case OP_TEXTURE_RECTANGLE:
            TRAP;
            break;
        case OP_TEXTURE_RECTANGLE_FLIP:
            TRAP;
            break;
        case OP_SYNC_PIPE:
            TRAP;
            break;
        case OP_SYNC_TILE:
            TRAP;
            break;
        case OP_SYNC_FULL:
            azRcpRaiseInterrupt(state, RCP_INTR_DP);
            break;
        case OP_SET_KEY_GB:
            TRAP;
            break;
        case OP_SET_KEY_R:
            TRAP;
            break;
        case OP_SET_CONVERT:
            TRAP;
            break;
        case OP_SET_SCISSOR:
            fa = fixed10_2((instr >> 44) & 0xfff);
            fb = fixed10_2((instr >> 32) & 0xfff);
            fc = fixed10_2((instr >> 12) & 0xfff);
            fd = fixed10_2((instr >>  0) & 0xfff);
            printf("Scissor: %f %f %f %f\n", fa, fb, fc, fd);
            break;
        case OP_SET_PRIM_DEPTH:
            TRAP;
            break;
        case OP_SET_OTHER_MODES:
            TRAP;
            break;
        case OP_LOAD_TLUT:
            TRAP;
            break;
        case OP_SYNC_LOAD:
            TRAP;
            break;
        case OP_SET_TILE_SIZE:
            TRAP;
            break;
        case OP_LOAD_BLOCK:
            TRAP;
            break;
        case OP_LOAD_TILE:
            TRAP;
            break;
        case OP_SET_TILE:
            TRAP;
            break;
        case OP_FILL_RECTANGLE:
            TRAP;
            break;
        case OP_SET_FILL_COLOR:
            TRAP;
            break;
        case OP_SET_FOG_COLOR:
            TRAP;
            break;
        case OP_SET_BLEND_COLOR:
            TRAP;
            break;
        case OP_SET_PRIM_COLOR:
            TRAP;
            break;
        case OP_SET_ENV_COLOR:
            TRAP;
            break;
        case OP_SET_COMBINE_MODE:
            TRAP;
            break;
        case OP_SET_TEXTURE_IMAGE:
            TRAP;
            break;
        case OP_SET_Z_IMAGE:
            TRAP;
            break;
        case OP_SET_COLOR_IMAGE:
            TRAP;
            break;
        }
    }
}

static void runCycles(AzState* state)
{
    uint32_t readIndex;
    uint32_t altReadIndex;

    readIndex = state->rdpCommandBuffer.readIndex;
    azLockMutex(state->rdpCommandBuffer.mutex + readIndex);

    for (;;)
    {
        runCommandBuffer(state, state->rdpCommandBuffer.data[readIndex], state->rdpCommandBuffer.size[readIndex]);
        state->rdpCommandBuffer.size[readIndex] = 0;
        altReadIndex = 1 - readIndex;
        azLockMutex(state->rdpCommandBuffer.mutex + altReadIndex);
        state->rdpCommandBuffer.readIndex = altReadIndex;
        azUnlockMutex(state->rdpCommandBuffer.mutex + readIndex);
        readIndex = altReadIndex;
        if (state->rdpCommandBuffer.size[readIndex] == 0)
        {
            break;
        }
    }
    state->rdpWorker.enabled = 0;
    azUnlockMutex(state->rdpCommandBuffer.mutex + readIndex);
}

void azRdpWorkerMain(void* s)
{
    AzState* state = (AzState*)s;

    for (;;)
    {
        azWorkerBarrier(&state->rdpWorker);
        runCycles(state);
    }
}
