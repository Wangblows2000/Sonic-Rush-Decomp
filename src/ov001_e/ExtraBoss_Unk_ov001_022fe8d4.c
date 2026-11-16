#include <nitro/types.h>

typedef struct UnkStruct_022fe8d4 {
    void *owner;
    u8 pad4[4];
    u32 flags;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u16 unk24;
} UnkStruct_022fe8d4;

void ExtraBoss_Unk_ov001_022fe8d4(UnkStruct_022fe8d4 *c, void *owner, u32 mask, s32 mode, s32 a, s32 b, s32 d, s32 e, s32 f, s32 g, u16 h) {
    c->owner = owner;
    c->flags = (mask & 7) | (mode == 1 ? 0x100 : 0);
    c->unk18 = a;
    c->unk1C = b;
    c->unk20 = d;
    c->unkC = e;
    c->unk10 = f;
    c->unk14 = g;
    c->unk24 = h;
}
