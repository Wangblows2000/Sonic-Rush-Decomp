#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x80];
    void *unk80;
} UnkStruct_0230f8c8_R;

void func_ov000_0230ef70(UnkStruct_StageObj *w);
void func_ov000_0230f474(UnkStruct_StageObj *w);

void func_ov000_0230f8c8(UnkStruct_StageObj *w) {
    if ((u32)w->unk8 < 0x1e)
        func_ov000_0230ef70(w);
    if (w->unk1BC != NULL && ((UnkStruct_0230f8c8_R *)w->unk1BC)->unk80 == w) {
        if (w->unk5C & 2)
            *(s8 *)&w->unk7 = -2;
        else
            *(s8 *)&w->unk7 = 2;
    }
    if (!(w->unk1E0->unk4 & 0x40) && w->unk1E0->unk2 != 0x54)
        return;
    if (w->unk1BC != NULL && ((UnkStruct_0230f8c8_R *)w->unk1BC)->unk80 == w)
        w->unk1F4 |= 1;
    if (!(w->unk1F4 & 1))
        return;
    w->unk8++;
    if (w->unk8 != 0x1e)
        return;
    if (w->unk1E0->unk2 == 0x54) {
        func_ov000_0230f474(w);
        return;
    }
    w->unk58 &= ~0x2000;
    w->unk58 |= 0x80;
    w->unk1C = w->unk10;
    w->unk20 = w->unk14;
}
