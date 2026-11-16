#include <ov000_c/stageobj.h>

void Boss7PartD5_Unk_ov000_022eb574(UnkStruct_StageObj *a, UnkStruct_StageObj *b) {
    s16 v;

    if (a->unk2E > 0)
        v = 0x800;
    else if (a->unk2E > 0)
        v = -0x800;
    else if (a->unk10 < b->unk10)
        v = 0x800;
    else
        v = -0x800;
    if (b->unk58 & 0x8000)
        b->unk2E = v;
    else
        b->unk34 = v;
    b->unk30 += b->unk30 >> 1;
    b->unk30 = -b->unk30;
    b->unk30 = b->unk30 < -0x500 ? -0x500 : (b->unk30 > 0x500 ? 0x500 : b->unk30);
}
