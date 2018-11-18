#include <stdio.h>
#include <libazote/libazote.h>

void azNextFrame(AzState* state)
{
    printf("\n\n\n\n\n***** FRAME START *****\n\n\n\n\n");
    pthread_mutex_lock(&state->viMutex);
    state->viSync = 0;
    azRcpRaiseInterrupt(state, RCP_INTR_VI);
    for (;;)
    {
        pthread_cond_wait(&state->viCond, &state->viMutex);
        if (state->viSync)
            break;
    }
    pthread_mutex_unlock(&state->viMutex);
    printf("\n\n\n\n\n***** FRAME END *****\n\n\n\n\n");
}
