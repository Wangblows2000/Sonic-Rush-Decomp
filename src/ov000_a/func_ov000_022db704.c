#include <nitro/types.h>

extern u32 data_0208fe70;

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

BOOL func_ov000_022db704(u32 mask, s32 n) {
    if (mask & (1 << (Rand() % n)))
        return TRUE;
    return FALSE;
}
