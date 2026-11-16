#include <nitro/types.h>

typedef struct UnkStruct_02312bcc {
    u8 pad0[0x16];
    u16 rot;
    u8 pad18[0x44 - 0x18];
    s32 unk44;
    u8 pad48[0x138 - 0x48];
    s32 speed;
} UnkStruct_02312bcc;

void func_ov001_02312bcc(UnkStruct_02312bcc *p, s32 target) {
    u32 a = p->rot;
    s32 d;

    if (a >= 0x8000)
        a -= 0x10000;
    if (a == target)
        return;
    d = target - a;
    if (d > p->speed)
        d = p->speed;
    else if (d < -p->speed)
        d = -p->speed;
    if (d == 0) {
        p->unk44 = 0;
        p->rot = target;
    } else {
        p->unk44 = d;
    }
}
