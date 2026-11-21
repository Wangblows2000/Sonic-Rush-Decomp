#include <nitro/types.h>

typedef struct UnkObj_02343310 UnkObj_02343310;
struct UnkObj_02343310 {
    u8 pad0[0xc];
    s32 unkC;
    s32 x;
    s32 y;
    u8 pad18[0x54 - 0x18];
    u32 unk54;
    u8 pad58[0x5c - 0x58];
    u16 unk5C;
    u8 pad5E[0x68 - 0x5e];
    void *unk68;
    u8 pad6C[0x88 - 0x6c];
    UnkObj_02343310 *parent;
    u8 pad8C[0x160 - 0x8c];
    u8 unk160[0xa];
    u16 unk16A;
};

void StageObj_HandleCollider(UnkObj_02343310 *o, void *p);
void ScreenShake_Start(s32 a);

void AttackHitbox_State_Boost(UnkObj_02343310 *o) {
    UnkObj_02343310 *p;

    if (o->unk16A & 0x200) {
        ScreenShake_Start(4);
        o->unk16A &= ~0x200;
    }
    p = o->parent;
    if (p != NULL) {
        o->x = p->x;
        o->y = o->parent->y;
        o->unk5C &= ~1;
        if (o->parent->unk5C & 1)
            o->unk5C |= 1;
        if (!(*(u32 *)((u8 *)p + 0x32c) & 0x80))
            o->unk54 |= 4;
        StageObj_HandleCollider(o, o->unk160);
        return;
    }
    o->unk54 |= 4;
}
