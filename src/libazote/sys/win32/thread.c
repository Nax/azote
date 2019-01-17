#include <libazote/libazote.h>

void azCreateThread(AzThread* thread, AzThreadEntrypoint entrypoint, void* arg)
{
    *thread = CreateThread(NULL, 0, entrypoint, arg, 0, NULL);
}

void azCreateMutex(AzMutex* mutex)
{
    InitializeCriticalSection(mutex);
}

void azLockMutex(AzMutex* mutex)
{
    EnterCriticalSection(mutex);
}

void azUnlockMutex(AzMutex* mutex)
{
    LeaveCriticalSection(mutex);
}

void azCreateConditionVariable(AzConditionVariable* cv)
{
    InitializeConditionVariable(cv);
}

void azWaitConditionVariable(AzConditionVariable* cv, AzMutex* mutex)
{
    SleepConditionVariableCS(cv, mutex, INFINITE);
}

void azSignalConditionVariable(AzConditionVariable* cv)
{
    WakeConditionVariable(cv);
}
