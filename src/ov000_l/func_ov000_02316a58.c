#include <ov000_b/stageobj.h>

void func_ov000_02316a1c(void);

void func_ov000_02316a58(UnkStruct_StageObj *w) {
    func_02069818(w, (u16)(w->unk8 + 1));
    w->unk68 = func_ov000_02316a1c;
    if (w->unk1E0->unk4 & 4)
        w->unk5C |= 0x20;
    else
        w->unk5C &= ~0x20;
    w->spr0.unk2C &= ~0x60000;
    w->spr1.unk2C &= ~0x60000;
    w->spr0.unk2C |= 0x40000;
    w->spr1.unk2C |= 0x40000;
}
