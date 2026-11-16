#include <ov000_c/stageobj.h>

s16 func_02068db4(s32 a, s32 b);
void func_ov000_02300808(UnkStruct_StageObj *w);
void func_ov000_0230023c(UnkStruct_StageObj *w);

void func_ov000_023005bc(UnkStruct_StageObj *w) {
    s16 a = 0x40;
    s16 sp = 0x100;
    u16 f;

    if (w->unk1E0->unk2 == 4)
        a = 4;
    f = w->unk1E0->unk4;
    if (f & 3) {
        sp += ((f & 0xc) >> 2) << 7;
        a = (a * sp) >> 8;
    }
    w->unk34 = func_02068db4(w->unk34, a);
    if (w->unk5C & 8) {
        w->unk5C ^= 1;
        func_ov000_02300808(w);
        return;
    }
    if (w->unk8 != 0) {
        w->unk8--;
        if (w->unk8 == 0)
            w->coll2.unkA |= 4;
    }
    if (w->unk1E0->unk2 == 0x10)
        func_ov000_0230023c(w);
}
