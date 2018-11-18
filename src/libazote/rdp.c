#include <libazote/libazote.h>

void* azRdpWorkerMain(void* s)
{
    AzState* state = (AzState*)s;

    pthread_setname_np("RDP Worker");
    for (;;)
    {
        azWorkerBarrier(&state->rdpWorker);
    }
}
