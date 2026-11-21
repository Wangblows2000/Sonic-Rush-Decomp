#include <nitro/types.h>

void Palette_StepUpToward(u16 *dst, const u16 *src, u16 n) {
    while (n-- != 0) {
        if ((*src & 0x1F) != (*dst & 0x1F)) *dst += 1;
        if ((*src & 0x3E0) != (*dst & 0x3E0)) *dst += 0x20;
        if ((*src & 0x7C00) != (*dst & 0x7C00)) *dst += 0x400;
        src++;
        dst++;
    }
}
