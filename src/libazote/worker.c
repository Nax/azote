#include <libazote/libazote.h>

void azWorkerBarrier(AzWorker* worker)
{
    if (!worker->enabled)
    {
        pthread_mutex_lock(&worker->mutex);
        if (!worker->enabled)
        {
            pthread_mutex_lock(&worker->mutexFeedback);
            worker->enabledFeedback = 0;
            pthread_mutex_unlock(&worker->mutexFeedback);
            pthread_cond_signal(&worker->condFeedback);
            for (;;)
            {
                pthread_cond_wait(&worker->cond, &worker->mutex);
                if (worker->enabled)
                    break;
            }
            worker->enabledFeedback = 1;
        }
        pthread_mutex_unlock(&worker->mutex);
    }
}

void azWorkerStart(AzWorker* worker)
{
    pthread_mutex_lock(&worker->mutex);
    worker->enabled = 1;
    pthread_mutex_unlock(&worker->mutex);
    pthread_cond_signal(&worker->cond);
}

void azWorkerStop(AzWorker* worker)
{
    pthread_mutex_lock(&worker->mutex);
    pthread_mutex_lock(&worker->mutexFeedback);
    worker->enabled = 0;
    pthread_mutex_unlock(&worker->mutex);
    for (;;)
    {
        pthread_cond_wait(&worker->cond, &worker->mutexFeedback);
        if (worker->enabledFeedback == 0)
            break;
    }
    pthread_mutex_unlock(&worker->mutexFeedback);
}
