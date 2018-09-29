#include <stdio.h>
#include <libazote/libazote.h>

static const char* const kRegisterNames[] = {
    "zero",
    "at",
    "v0",
    "v1",
    "a0",
    "a1",
    "a2",
    "a3",
    "t0",
    "t1",
    "t2",
    "t3",
    "t4",
    "t5",
    "t6",
    "t7",
    "s0",
    "s1",
    "s2",
    "s3",
    "s4",
    "s5",
    "s6",
    "s7",
    "t8",
    "t9",
    "k0",
    "k1",
    "gp",
    "sp",
    "s8",
    "ra",
};

void azDebugDumpState(AzState* state)
{
    puts("==================== DEBUG ====================");
    puts("CPU:");
    for (size_t i = 0; i < 32; ++i)
    {
        const char* const regName = kRegisterNames[i];
        printf("  %s:", regName);
        for (int j = 10 - strlen(regName); j > 0; --j)
        {
            putchar(' ');
        }
        printf("0x%016llx\n", state->cpu.registers[i]);
    }
    printf("  pc:        0x%016llx\n", state->cpu.pc);
    puts("===============================================");
    puts("");
}

void azDebugHexdumpRaw(char* addr, size_t len)
{
    uint8_t data;

    size_t i = 0;
    for (;;)
    {
        if (i >= len)
            break;
        data = *(uint8_t*)(addr + i);
        i++;
        printf("%02x ", data);
        if (i % 16 == 0)
            puts("");
        else if (i % 8 == 0)
            putchar(' ');
    }
    puts("");
}
