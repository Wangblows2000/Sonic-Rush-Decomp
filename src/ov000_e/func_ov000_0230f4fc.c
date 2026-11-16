#include <ov000_c/stageobj.h>

void func_020694b0(UnkStruct_StageObj *w);
void func_ov000_0230f4c8(void);

void func_ov000_0230f4fc(UnkStruct_StageObj *w) {
    s32 i;

    w->unk42 >>= 3;
    w->unk42 += w->unk1E0->unk7 << 6;
    w->unk58 &= ~0x2000;
    w->unk58 |= 0x80;
    if (w->unk1E0->unk8 != 0) {
        for (i = 0; i < w->unk1E0->unk8; i++)
            func_020694b0(w);
    }
    w->unk68 = func_ov000_0230f4c8;
}
