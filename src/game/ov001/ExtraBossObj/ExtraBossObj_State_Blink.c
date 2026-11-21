#include <nitro/types.h>

void ExtraBossObj_State_Blink(u8 *p) {
    *(u32 *)(p + 0x14) ^= 0x30000;
}
