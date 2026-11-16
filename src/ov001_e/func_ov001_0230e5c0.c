#include <nitro/types.h>

typedef struct UnkStruct_0230e5c0 {
    u8 pad0[0x16];
    u16 rot;
    u8 pad18[4];
    s32 unk1C;
    u8 pad20[0xd0 - 0x20];
    s32 timer;
} UnkStruct_0230e5c0;

extern s16 data_0206e470[];

#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void func_ov001_0230e5c0(UnkStruct_0230e5c0 *p) {
    s32 d = p->timer - p->unk1C / 0x5000;
    u16 a;

    if (d < 0)
        return;
    a = d * 800;
    p->rot += COS(a) / 8;
}
