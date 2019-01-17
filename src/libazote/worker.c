#include <libazote/libazote.h>

int azWorkerBarrier(AzWorker* worker)
{
    if (!worker->enabled)
    {
        azLockMutex(&worker->mutex);
        if (!worker->enabled)
        {
            azLockMutex(&worker->mutexFeedback);
            worker->enabledFeedback = 0;
            azUnlockMutex(&worker->mutexFeedback);
            azSignalConditionVariable(&worker->condFeedback);
            for (;;)
            {
                azWaitConditionVariable(&worker->cond, &worker->mutex);
                if (worker->enabled)
                    break;
            }
            worker->enabledFeedback = 1;
        }
        azUnlockMutex(&worker->mutex);
        return 1;
    }
    return 0;
}

void azWorkerStart(AzWorker* worker)
{
    azLockMutex(&worker->mutex);
    worker->enabled = 1;
    azUnlockMutex(&worker->mutex);
    azSignalConditionVariable(&worker->cond);
}

void azWorkerStop(AzWorker* worker)
{
    azLockMutex(&worker->mutex);
    azLockMutex(&worker->mutexFeedback);
    worker->enabled = 0;
    azUnlockMutex(&worker->mutex);
    for (;;)
    {
        azWaitConditionVariable(&worker->cond, &worker->mutexFeedback);
        if (worker->enabledFeedback == 0)
            break;
    }
    azUnlockMutex(&worker->mutexFeedback);
}
