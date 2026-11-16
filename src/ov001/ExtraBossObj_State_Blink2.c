#include <nitro/types.h>

void ExtraBossObj_State_Blink2(u8 *p) {
    *(u32 *)(p + 0x14) ^= 0x30000;
}
