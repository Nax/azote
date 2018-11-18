#include <libazote/libazote.h>

void* azAudioWorkerMain(void* s)
{
    AzState* state = (AzState*)s;

    pthread_setname_np("Audio Worker");

    for (;;)
    {
        azWorkerBarrier(&state->audioWorker);
    }
}
