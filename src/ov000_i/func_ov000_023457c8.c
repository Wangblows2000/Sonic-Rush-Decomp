#include <nitro/types.h>

extern s16 data_0206e470[];
s16 func_02068df8(s32 v, s32 a, s32 b);

#define SIN_023457c8(a) data_0206e470[((s32)((u16)(a) & 0xffff) >> 4) * 2]

void func_ov000_023457c8(u8 *w, u8 *o) {
    u8 ang = w[1];

    if (o != NULL)
        ang = o[1];
    *(s16 *)(w + 0x34) = func_02068df8(*(s16 *)(w + 0x34), (*(s16 *)(w + 0x3c) * SIN_023457c8(ang << 8)) >> 12, *(s16 *)(w + 0x3e));
    *(u32 *)(w + 0x32c) |= 0x8000000;
}
