#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_Hit;

void func_ov000_022fcd10(UnkStruct_StageObj *w);
void func_ov000_02355b20(UnkStruct_StageObj *p, s16 a, s16 b);
void func_ov000_02355da0(UnkStruct_StageObj *p, UnkStruct_StageObj *w);

void func_ov000_022fc6e8(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_StageObj *w = a->unk18;
    UnkStruct_StageObj *p = b->unk18;

    if (w == NULL)
        return;
    if (p == NULL)
        return;
    func_ov000_022fcd10(w);
    {s32 y = -0x780; y -= w->unk1E0->unk7 * 0x180;
    func_ov000_02355b20(p, 0, y);}
    func_ov000_02355da0(p, w);
}
