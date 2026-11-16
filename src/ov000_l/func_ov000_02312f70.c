#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x28];
    s16 unk28;
    u8 pad2A[0x37c - 0x2a];
    s16 unk37C;
} UnkPlayer_02312f70;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u32 unkC;
    s32 unk10;
    u8 pad14[0x34 - 0x14];
    s16 unk34;
    u8 pad36[0x14c - 0x36];
    s32 unk14C;
    u8 pad150[0x1e0 - 0x150];
    UnkStruct_ObjParam *unk1E0;
    u8 pad1E4[0x1f4 - 0x1e4];
    u32 unk1F4;
} UnkWork_02312f70;

void func_ov000_02312f70(UnkWork_02312f70 *w, UnkPlayer_02312f70 *p) {
    s32 v;

    w->unk34 = p->unk28;
    v = w->unk34;
    if (v < 0)
        v = -v;
    if (p->unk37C <= v)
        w->unk1F4 |= 1;
    else
        w->unk1F4 &= ~1;
    if (w->unk1E0->unk4 & 2) {
        if (w->unk34 < 0x800)
            w->unk34 = 0x800;
        if (w->unk1E0->unk6 < 0)
            w->unk34 = -w->unk34;
        if (w->unk8 == w->unk10)
            w->unk34 = -w->unk34;
        w->unk1F4 |= 1;
    }
    w->unk14C = 0;
    w->unkC &= ~0xff;
}
