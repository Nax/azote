#include <stdio.h>
#include <libazote/libazote.h>

static void dmaAudio(AzState* state)
{
    static uint16_t audioTmpBuffer[8192];

    uint32_t ramAddr = state->aiRegisters[0];
    uint32_t len = state->aiRegisters[1];
    uint16_t* src = (uint16_t*)(state->rdram + ramAddr);

    for (size_t i = 0; i < len * 2; ++i)
    {
        audioTmpBuffer[i] = bswap16(src[i]);
    }

    //puts("\n\n--- AUDIO ---\n\n");
    alBufferData(state->audioBuffers[0], AL_FORMAT_MONO16, audioTmpBuffer, len * 4, 32006);
    alSourceQueueBuffers(state->audioSource, 1, state->audioBuffers);
    alSourcePlay(state->audioSource);
    //state->aiRegisters[1] = 0;
    //azRcpRaiseInterrupt(state, RCP_INTR_AI);
}

uint32_t azRcpReadAI(AzState* state, uint32_t addr)
{
    ALenum sourceState;

    //printf("AI Read:  0x%08x\n", addr);
    switch (addr)
    {
    case AI_DRAM_ADDR_REG:
        return 0;
    case AI_LEN_REG:
        return state->aiRegisters[1];
    case AI_CONTROL_REG:
        return 0;
    case AI_STATUS_REG:
        alGetSourcei(state->audioSource, AL_SOURCE_STATE, &sourceState);
        if (sourceState == AL_PLAYING)
            return 0xc0000001;
        else
            return 0;
        return state->aiRegisters[3];
    case AI_DACRATE_REG:
        return 0;
    case AI_BITRATE_REG:
        return 0;
    default:
        return 0;
    }
}

void azRcpWriteAI(AzState* state, uint32_t addr, uint32_t value)
{
    //printf("AI Write: 0x%08x (0x%08x)\n", addr, value);
    switch (addr)
    {
    case AI_DRAM_ADDR_REG:
        state->aiRegisters[0] = value;
        return;
    case AI_LEN_REG:
        state->aiRegisters[1] = value;
        if (state->aiRegisters[2])
            dmaAudio(state);
        return;
    case AI_CONTROL_REG:
        state->aiRegisters[2] = value;
        return;
    case AI_STATUS_REG:
        azRcpClearInterrupt(state, RCP_INTR_AI);
        return;
    case AI_DACRATE_REG:
        state->aiRegisters[4] = value;
        return;
    case AI_BITRATE_REG:
        state->aiRegisters[5] = value;
        return;
    default:
        return;
    }
}
