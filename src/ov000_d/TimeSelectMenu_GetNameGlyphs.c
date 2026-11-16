#include <nitro/types.h>

u32 Font_GetGlyphIndex(u16 c);

#pragma thumb on

void TimeSelectMenu_GetNameGlyphs(u32 *out, u16 *src, s32 n) {
    s32 i;

    for (i = 0; i < n && i < 6; i++) {
        out[i] = Font_GetGlyphIndex(src[i]);
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
