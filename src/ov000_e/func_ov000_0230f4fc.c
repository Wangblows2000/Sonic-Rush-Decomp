#include <ov000_c/stageobj.h>

void StageObj_Move(UnkStruct_StageObj *w);
void GmkLand_State_Unk_ov000_0230f4c8(void);

void func_ov000_0230f4fc(UnkStruct_StageObj *w) {
    s32 i;

    w->unk42 >>= 3;
    w->unk42 += w->unk1E0->unk7 << 6;
    w->unk58 &= ~0x2000;
    w->unk58 |= 0x80;
    if (w->unk1E0->unk8 != 0) {
        for (i = 0; i < w->unk1E0->unk8; i++)
            StageObj_Move(w);
    }
    w->unk68 = GmkLand_State_Unk_ov000_0230f4c8;
}
