#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_Hit;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x2A0 - 0x20C];
    u32 unk2A0;
    u32 unk2A4;
} UnkStruct_02306d88;

extern s16 data_0206e470[];

void func_ov000_023557c8(UnkStruct_StageObj *p, s32 x, s32 y, s16 vx, s16 vy);
void func_ov000_023497a4(UnkStruct_StageObj *p);
void func_ov000_02355da0(UnkStruct_StageObj *p, UnkStruct_02306d88 *w);

void func_ov000_02306d88(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_02306d88 *w = a->unk18;
    u16 ang = (u8)w->unk2A4 << 13;
    s32 i = (s32)(ang & 0xffff) >> 4;
    s16 vx = (data_0206e470[(u32)((i << 1) + 1)] * 0x800) >> 12;
    s16 vy = (data_0206e470[(u32)(i << 1)] * 0x800) >> 12;

    if (b->unk18 == NULL)
        return;
    if (w->unk2A0 & 1) {
        func_ov000_023557c8(b->unk18, w->base.unk10, w->base.unk14, vx, vy);
        func_ov000_023497a4(b->unk18);
    } else {
        func_ov000_023557c8(b->unk18, w->base.unk10, w->base.unk14, vx, vy);
    }
    func_ov000_02355da0(b->unk18, w);
}
