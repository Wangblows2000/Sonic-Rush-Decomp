#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_023661a8[][3];

void func_020693f0(s32 a, s16 *b, s32 c);
UnkStruct_StageObj *func_ov000_023263c0(s32 kind, s32 x, s32 y);

void func_ov000_022fb738(UnkStruct_StageObj *w) {
    s16 dy = 0;
    u8 kind;
    UnkStruct_StageObj *o;

    if (w->unk1F4 & 0x20)
        return;
    if (data_022c4560.unk1C < 8)
        kind = data_ov000_023661a8[data_022c4560.unk1C][Rand() % 3];
    else
        kind = Rand() & 0xf;
    dy = 0x800;
    func_020693f0(0, &dy, w->unk5);
    o = func_ov000_023263c0(kind, w->unk10, w->unk14 - dy);
    if (o == NULL)
        return;
    o->unk5 = w->unk5;
    if (!(w->unk58 & 0x1000) && !(w->unk1F4 & 0x40))
        return;
    if (w->unk1F4 & 0x40)
        o->unk8 = 1;
    else
        o->unk58 |= 0x100;
}
