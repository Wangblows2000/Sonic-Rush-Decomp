#include <ov000_c/stageobj.h>

typedef struct UnkStruct_022e8bac {
    UnkStruct_StageObj base;
    u8 pad20C[0x3D0 - 0x20C];
    s32 unk3D0;
    u8 pad3D4[0x3D8 - 0x3D4];
    u16 unk3D8;
    u8 pad3DA[0x440 - 0x3DA];
    void (*unk440)(struct UnkStruct_022e8bac *);
} UnkStruct_022e8bac;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x220 - 0x20C];
    u32 unk220;
} UnkStruct_Boss;

typedef struct {
    u8 pad[0x114];
    s32 unk114;
} UnkStruct_124;

extern UnkStruct_Boss *data_ov000_02369b78;

void func_ov000_02328468(s32 a, s32 x, s32 y, s32 z);
void func_ov000_022eb3c4(UnkStruct_022e8bac *w);
void func_ov000_022e899c(UnkStruct_022e8bac *w);

void func_ov000_022e8bac(UnkStruct_022e8bac *w) {
    UnkStruct_Boss *b = data_ov000_02369b78;

    w->base.spr0.unk2A += 4;
    w->base.spr1.unk2A += 4;
    ((UnkStruct_124 *)w->base.unk124)->unk114 += 0x400;
    if (w->unk3D0 == 0) {
        if ((b->unk220 & 0x1f) == 0)
            func_ov000_02328468(1, w->base.unk10, w->base.unk14, w->base.unk18 + 8);
    } else {
        if ((b->unk220 & 7) == 0)
            func_ov000_022eb3c4(w);
    }
    if (w->base.spr0.unk2A < 0x80)
        return;
    if (w->unk3D8 & 0x100)
        return;
    w->base.spr0.unk2A = 0x80;
    w->base.spr1.unk2A = 0x80;
    ((UnkStruct_124 *)w->base.unk124)->unk114 = 0x8000;
    w->unk440 = func_ov000_022e899c;
    w->unk440(w);
}
