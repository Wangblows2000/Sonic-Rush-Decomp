#include <ov000_c/stageobj.h>

extern UnkStruct_StageObj *data_ov000_0236ac60;

void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void Sound_SetPositional(s32 a, UnkStruct_StageObj *w, UnkStruct_StageObj *p);
void func_0201bde0(s32 a, s32 b, s32 c);
void func_ov000_0232eaac(UnkStruct_StageObj *w, s32 a, s32 b);
void func_ov000_022fdf28(UnkStruct_StageObj *w);
void func_ov000_022fdb9c(UnkStruct_StageObj *w);
void func_ov000_022fd8d8(UnkStruct_StageObj *w);
void func_ov000_022fd744(UnkStruct_StageObj *w);
void func_ov000_022fd564(UnkStruct_StageObj *w);
void func_ov000_022fd370(UnkStruct_StageObj *w);
void func_ov000_022fd19c(UnkStruct_StageObj *w);
void func_ov000_022fd030(UnkStruct_StageObj *w);
void func_ov000_022fcc50(UnkStruct_StageObj *w);

void func_ov000_022fc894(UnkStruct_Coll *a, UnkStruct_Coll *b) {
    UnkStruct_StageObj *w = a->unk18;
    UnkStruct_StageObj *o = b->unk18;

    if (w == NULL)
        return;
    if (o == NULL)
        return;
    if (w->unk54 & 2)
        return;
    if (o == NULL)
        return;
    Sound_PlaySeqArcEx(w->unk1DC, 0, 0x79);
    Sound_SetPositional(w->unk1DC, w, data_ov000_0236ac60);
    func_0201bde0(w->unk1DC, 0xffff, 0x400);
    func_ov000_0232eaac(w, 0, -0x40);
    w->unk204 = o;
    if (w->unk10 > o->unk10)
        w->unk5C |= 1;
    else
        w->unk5C &= ~1;
    if (w->unk5 != 0)
        w->unk5C ^= 1;
    switch (w->unk1E0->unk2) {
    case 0:
    case 0x14:
        func_ov000_022fdf28(w);
        return;
    case 3:
        func_ov000_022fdb9c(w);
        return;
    case 1:
        func_ov000_022fd8d8(w);
        return;
    case 7:
    case 0x15:
        func_ov000_022fd744(w);
        return;
    case 8:
        func_ov000_022fd564(w);
        return;
    case 9:
        func_ov000_022fd370(w);
        return;
    case 10:
        func_ov000_022fdf28(w);
        return;
    case 0xd:
        func_ov000_022fd19c(w);
        return;
    case 0xe:
        func_ov000_022fd030(w);
        return;
    case 0xf:
        w->unk204 = o;
        func_ov000_022fcc50(w);
        break;
    }
}
