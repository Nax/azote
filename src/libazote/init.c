#include <libazote/libazote.h>

#define zalloc(x)   calloc((x), 1)

static void initWorker(AzWorker* worker)
{
    pthread_cond_init(&worker->cond, NULL);
    pthread_mutex_init(&worker->mutex, NULL);
}

AzState* azInit()
{
    AzState* state;

    state = zalloc(sizeof(*state));

    state->rdram = zalloc(AZOTE_MEMORY_SIZE);
    state->rdramRegisters = zalloc(0x28);
    state->spDmem = zalloc(0x1000);
    state->spImem = zalloc(0x1000);
    state->miRegisters = zalloc(0x10);
    state->viRegisters = zalloc(0x38);
    state->aiRegisters = zalloc(0x18);
    state->piRegisters = zalloc(0x34);
    state->riRegisters = zalloc(0x20);
    state->siRegisters = zalloc(0x1c);
    state->pifram = zalloc(0x40);

    for (int i = 0; i < 2; ++i)
    {
        pthread_mutex_init(state->rdpCommandBuffer.mutex + i, NULL);
        state->rdpCommandBuffer.capacity[i] = 128;
        state->rdpCommandBuffer.size[i] = 0;
        state->rdpCommandBuffer.data[i] = malloc(128);
    }

    initWorker(&state->cpuWorker);
    initWorker(&state->rspWorker);
    initWorker(&state->rdpWorker);
    pthread_create(&state->cpuWorker.thread, NULL, &azCpuWorkerMain, state);
    pthread_create(&state->rspWorker.thread, NULL, &azRspWorkerMain, state);
    pthread_create(&state->rdpWorker.thread, NULL, &azRdpWorkerMain, state);

    return state;
}

void azExit(AzState* state)
{
    if (state)
    {
        free(state->cart);
        free(state->rdram);
        free(state->rdramRegisters);
        free(state->spDmem);
        free(state->spImem);
        free(state->miRegisters);
        free(state->viRegisters);
        free(state->aiRegisters);
        free(state->piRegisters);
        free(state->riRegisters);
        free(state->siRegisters);
        free(state->pifram);
    }
    free(state);
}

void azRun(AzState* state)
{
    azWorkerStart(&state->cpuWorker);

    for (;;)
    {
    }
}
