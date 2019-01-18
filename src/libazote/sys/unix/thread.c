#include <libazote/libazote.h>

void azCreateThread(AzThread* thread, AzThreadEntrypoint entrypoint, void* arg)
{
    pthread_create(thread, NULL, (void*)entrypoint, arg);
}

void azCreateMutex(AzMutex* mutex)
{
    pthread_mutex_init(mutex, NULL);
}

void azLockMutex(AzMutex* mutex)
{
    pthread_mutex_lock(mutex);
}

void azUnlockMutex(AzMutex* mutex)
{
    pthread_mutex_unlock(mutex);
}

void azCreateConditionVariable(AzConditionVariable* cv)
{
    pthread_cond_init(cv, NULL);
}

void azWaitConditionVariable(AzConditionVariable* cv, AzMutex* mutex)
{
    pthread_cond_wait(cv, mutex);
}

void azSignalConditionVariable(AzConditionVariable* cv)
{
    pthread_cond_signal(cv);
}
