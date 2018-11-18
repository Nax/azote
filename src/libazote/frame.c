#include <stdio.h>
#include <libazote/libazote.h>

void azNextFrame(AzState* state)
{
    azRcpRaiseInterrupt(state, RCP_INTR_VI);
    azRunCPU(state, 1562500);
}
