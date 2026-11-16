#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_022c4680;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_022c4680 gSaveData;
extern UnkStruct_StageObj *data_ov000_0236ac60;

void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void Sound_SetPositional(s32 a, UnkStruct_StageObj *w, UnkStruct_StageObj *p);
void func_ov000_022fc5b8(UnkStruct_StageObj *w, s32 a, s32 b, s32 c);
void func_ov000_022fe7e8(UnkStruct_StageObj *w);
void func_ov000_022faf48(UnkStruct_StageObj *w, s32 a);

void func_ov000_022fd5f8(UnkStruct_StageObj *w) {
    if ((w->spr0.unkE >> 4) == 0x15) {
        if (!(w->unk1F4 & 4)) {
            Sound_PlaySeqArcEx(w->unk1DC, 0, 0x74);
            Sound_SetPositional(w->unk1DC, w, data_ov000_0236ac60);
            func_ov000_022fc5b8(w, 0xe4, 0x30, -0x23);
        }
        w->unk1F4 |= 4;
    }
    if (!(w->unk1F4 & 1)) {
        if (w->unkC == 0)
            return;
        w->unkC--;
        if (w->unkC != 0)
            return;
        func_ov000_022faf48(w, 2);
        w->unk5C |= 4;
        w->unk1F4 |= 1;
        w->unkC = 0x3a;
        if ((gSaveData.unk8 & 1) && data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
            w->unkC += w->unkC >> 1;
        return;
    }
    w->unkC--;
    if (w->unkC == 0) {
        w->unk1F4 &= ~5;
        func_ov000_022fe7e8(w);
    }
}
