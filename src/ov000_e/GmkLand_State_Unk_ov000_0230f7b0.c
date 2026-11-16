#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x80];
    void *unk80;
} UnkStruct_0230f7b0_R;

void GmkLand_Action_Unk_ov000_0230f474(UnkStruct_StageObj *w);

void GmkLand_State_Unk_ov000_0230f7b0(UnkStruct_StageObj *w) {
    s16 n = 0x1e;

    if (w->unk1E0->unk6 > 0)
        n = w->unk1E0->unk6 << 2;
    if (w->unk1BC != NULL && ((UnkStruct_0230f7b0_R *)w->unk1BC)->unk80 == w)
        w->unk7 = 2;
    if (w->unk1E0->unk4 & 1)
        w->unk1F4 &= ~1;
    if (w->unk1BC != NULL && ((UnkStruct_0230f7b0_R *)w->unk1BC)->unk80 == w)
        w->unk1F4 |= 1;
    if (w->unk1F4 & 1) {
        w->unk8++;
        if (w->unk8 != n)
            return;
        GmkLand_Action_Unk_ov000_0230f474(w);
        return;
    }
    w->unk8 = 0;
}
