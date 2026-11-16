#include <nitro/types.h>

typedef struct UnkStruct_0230e554 {
    u8 pad0[0x18];
    s32 x;
    s32 unk1C;
    u8 pad20[0xd0 - 0x20];
    s32 timer;
} UnkStruct_0230e554;

extern s16 data_0206e470[];

#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void func_ov001_0230e554(UnkStruct_0230e554 *p) {
    s32 d = p->timer - p->unk1C / 0x5000;
    u16 a;

    if (d < 0)
        return;
    a = d * 800;
    p->x += COS(a);
}
