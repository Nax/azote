#include <stdio.h>
#include <libazote/libazote.h>

int azRomGetCIC(void* cart)
{
    uint32_t crc = azCRC32((char*)cart + 0x40, 0xfc0);
    printf("CIC Hash: 0x%08x\n", crc);

    switch (crc)
    {
    case 0x90bb6cb5:
        return CIC_6102;
    case 0x0b050ee0:
        return CIC_6103;
    case 0x98bc2c86:
        return CIC_6105;
    default:
        return CIC_UNKNOWN;
    }
}
