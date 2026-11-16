#include <nitro/types.h>

extern u16 gFontGlyphRangeStartTable[];
extern s32 gFontGlyphRangeLengthTable[];
extern s32 gFontGlyphRangeBaseTable[];

s32 Font_GetGlyphIndex(u32 code)
{
    s32 lo = 0;
    s32 hi = 0x38;
    s32 d;
    do {
        s32 s = lo + hi;
        s32 mid = (s & 1) + (s >> 1);
        if (gFontGlyphRangeStartTable[mid] <= code)
            lo = mid;
        else
            hi = mid - 1;
    } while (lo < hi);
    d = code - gFontGlyphRangeStartTable[lo];
    if (d >= gFontGlyphRangeLengthTable[lo])
        return 0;
    return d + gFontGlyphRangeBaseTable[lo];
}
