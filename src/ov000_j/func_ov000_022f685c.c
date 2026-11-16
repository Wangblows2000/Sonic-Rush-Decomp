#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    u8 padC[0x144 - 0xc];
} UnkStruct_022f685c_e;

typedef struct {
    u8 pad0[0x218];
    UnkStruct_022f685c_e elems[2];
    u8 pad4A0[0x4b0 - 0x4a0];
    u16 unk4B0;
} UnkStruct_022f685c;

void BossEffect_CreateBeamFinB(s32 a, s32 x, s32 y, s32 z);

void func_ov000_022f685c(UnkStruct_022f685c *p) {
    s32 i;

    switch (p->unk4B0++ & 7) {
    case 0:
        i = 0;
        break;
    case 4:
        i = 1;
        break;
    default:
        return;
    }
    BossEffect_CreateBeamFinB(1, p->elems[i].x, p->elems[i].y, p->elems[i].z);
}
