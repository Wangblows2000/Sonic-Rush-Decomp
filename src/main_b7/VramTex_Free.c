#include <nitro/types.h>

extern u16 data_0208a77e[];
u32 func_02006abc(u32 a);

void VramTex_Free(u32 addr) {
    u32 idx;
    if (addr == func_02006abc(0))
        return;
    idx = addr >> 9;
    if (data_0208a77e[idx] & 0x8000) {
        u32 j;
        if (idx < 0x100)
            j = (idx >> 1) + 0x100;
        else
            j = ((idx - 0x100) >> 1) + 0x100;
        data_0208a77e[j] = 0;
    }
    data_0208a77e[idx] = 0;
}
