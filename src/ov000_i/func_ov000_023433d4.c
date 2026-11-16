#include <nitro/types.h>

typedef struct UnkObj_023433d4 UnkObj_023433d4;
struct UnkObj_023433d4 {
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
    UnkObj_023433d4 *parent;
    u8 pad8C[0x160 - 0x8c];
    u8 unk160[0xa];
    u16 unk16A;
};

void StageObj_HandleCollider(UnkObj_023433d4 *o, void *p);

void func_ov000_023433d4(UnkObj_023433d4 *o) {
    if (o->unkC != 0) {
        o->unkC--;
        if (o->unkC == 0) {
            o->unk54 |= 4;
            return;
        }
        StageObj_HandleCollider(o, o->unk160);
        return;
    }
    if (o->parent != NULL) {
        o->x = o->parent->x;
        o->y = o->parent->y;
        o->unk5C &= ~1;
        if (o->parent->unk5C & 1)
            o->unk5C |= 1;
        StageObj_HandleCollider(o, o->unk160);
        return;
    }
    o->unk54 |= 4;
}
