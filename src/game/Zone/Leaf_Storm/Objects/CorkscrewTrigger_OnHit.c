#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x34 - 1];
    s16 unk34;
    u8 pad36[0x328 - 0x36];
    s16 unk328;
    u8 pad32A[0x37C - 0x32A];
    s16 unk37C;
} UnkStruct_02315c50_P;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x1E0 - 0x18];
    UnkStruct_ObjParam *unk1E0;
} UnkStruct_02315c50_O;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_02315c50_C;

void Player_Action_CorkscrewPath(UnkStruct_02315c50_P *p, s32 x, s32 y, u16 f);

void CorkscrewTrigger_OnHit(UnkStruct_02315c50_C *a, UnkStruct_02315c50_C *b) {
    UnkStruct_02315c50_O *o = a->unk18;
    UnkStruct_ObjParam *prm = o->unk1E0;
    u16 f = prm->unk4;
    u16 g = f & ~0x80;
    UnkStruct_02315c50_P *p;
    s32 lim;

    if (prm->unk2 == 0x69)
        g |= prm->unk6 << 2;
    p = b->unk18;
    if (p == NULL)
        return;
    if (p->unk0 != 1)
        return;
    lim = p->unk37C;
    if (f & 0x80)
        lim = 0;
    if ((p->unk34 >= lim && !(f & 1)) || (p->unk34 <= -lim && (f & 1))) {
        if (prm->unk2 == 0x69)
            g |= 0x1000;
        if (f & 0x80)
            g |= 0x2000;
        if (f & 0x80) {
            if (p->unk328 >= 0x1a && p->unk328 <= 0x20)
                Player_Action_CorkscrewPath(p, o->unk10, o->unk14, g);
        } else {
            Player_Action_CorkscrewPath(p, o->unk10, o->unk14, g);
        }
    }
}
