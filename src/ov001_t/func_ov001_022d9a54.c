#include <nitro/types.h>

u32 Font_GetGlyphIndex(u16 id);

#pragma thumb on
void func_ov001_022d9a54(u32 *out, u16 *ids, s32 n) {
    s32 i = 0;

    while (i < n && i < 6) {
        out[i] = Font_GetGlyphIndex(*ids);
        ids++;
        i++;
    }
    if ((u32)n < 7) {
        for (; i < 7; i++) {
            out[i] = Font_GetGlyphIndex(0x20);
        }
    } else {
        out[6] = Font_GetGlyphIndex(0x2026);
    }
}
#pragma thumb reset
