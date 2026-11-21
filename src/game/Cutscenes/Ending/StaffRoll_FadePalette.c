#include <nitro/types.h>

typedef struct UnkStruct_022cfecc {
    u8 pad0[0x14];
    u32 unk14;
    u32 unk18;
} UnkStruct_022cfecc;

void PaletteTransfer_QueueCopy(u16 *src, u16 n, u32 a, u32 b);

#pragma thumb on

void StaffRoll_FadePalette(UnkStruct_022cfecc *p, s32 n, float scale, u16 *src, u16 *dst) {
    s32 i;
    for (i = 0; i < n; i++) {
        u16 c = src[i];
        s32 r = c & 0x1f;
        s32 g = (c >> 5) & 0x1f;
        s32 b = (c >> 10) & 0x1f;
        r = (s32)((float)r * scale);
        g = (s32)((float)g * scale);
        b = (s32)((float)b * scale);
        dst[i] = (r & 0x1f) | ((g & 0x1f) << 5) | ((b & 0x1f) << 10);
    }
    PaletteTransfer_QueueCopy(dst, n, p->unk14, p->unk18);
}

#pragma thumb reset
