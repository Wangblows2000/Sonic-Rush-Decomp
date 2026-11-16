#include <nitro/types.h>

typedef struct UnkStruct_022cfd7c {
    u8 pad0[0x64];
    u32 unk64;
    u32 unk68;
    u32 unk6C;
    u32 unk70;
    u16 unk74;
    u16 unk76;
} UnkStruct_022cfd7c;

void PaletteTransfer_QueueCopy(u16 *src, u16 n, u32 a, u32 b);

#pragma thumb on

void func_ov001_022cfd7c(UnkStruct_022cfd7c *p, s32 n, float scale, u16 *src, u16 *dst) {
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
    PaletteTransfer_QueueCopy(dst, n, p->unk64, p->unk6C + n * (p->unk74 * 2));
    PaletteTransfer_QueueCopy(dst, n, p->unk68, p->unk70 + n * (p->unk76 * 2));
}

#pragma thumb reset
