#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_Hit;

BOOL ObjRect_BoxCheck(UnkStruct_Hit *h, s32 x, s32 y, s32 a, s32 b, s32 c, s32 d);
void func_ov000_02351950(UnkStruct_StageObj *p, UnkStruct_StageObj *w, s32 x0, s32 y, s32 x1);

void func_ov000_02301ac4(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_StageObj *w = a->unk18;
    UnkStruct_StageObj *p = b->unk18;
    s32 dy;

    if (w == NULL)
        return;
    if (p == NULL)
        return;
    if (!ObjRect_BoxCheck(a, p->unk10 >> 8, p->unk14 >> 8, -6, -0x10, 6, 0x10))
        return;
    if (p->unk0 != 1)
        return;
    dy = w->unk1E0->unk7 << 8;
    if (w->unk5C & 2)
        dy = -dy;
    func_ov000_02351950(p, w, w->unk10 + ((w->unk1E0->unk6 - 6) << 8), w->unk14 + dy,
                        w->unk10 + ((w->unk1E0->unk6 + w->unk1E0->unk8 + 6) << 8));
}
