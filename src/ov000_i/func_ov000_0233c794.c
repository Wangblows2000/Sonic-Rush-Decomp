#include <nitro/types.h>

BOOL func_ov000_0233c794(u8 *w) {
    u16 *p;
    u16 t;
    u8 j;
    u8 i;

    (*(s16 *)(w + 0x8b8))++;
    if (*(s16 *)(w + 0x8b8) >= 3) {
        *(s16 *)(w + 0x8b8) = 0;
    for (i = 0; i < 2; i++) {
        p = (i == 0) ? (u16 *)(w + 0xee) : (u16 *)(w + 0x2ee);
        t = p[0];
        for (j = 0; j < 4; j++)
            p[j] = p[j + 1];
        p[j] = t;
    }
    return TRUE;
    }
    return FALSE;
}
