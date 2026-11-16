#include <ov000_c/stageobj.h>

void ScreenShake_Start(s32 a);
UnkStruct_StageObj *func_ov000_0232dedc(UnkStruct_StageObj *w, s16 a, s16 b, s16 c, s16 d, u8 e);
void func_ov000_0232e48c(UnkStruct_StageObj *w, s16 x, s16 y);
void func_0201c070(s32 a, s32 b);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void func_ov000_0230e50c(void);

void func_ov000_0230e5dc(UnkStruct_StageObj *w) {
    w->unk68 = func_ov000_0230e50c;
    w->unk58 |= 0x100;
    w->unk58 &= ~1;
    w->unk58 |= 0x10;
    w->unk58 &= ~0x40;
    w->unk5C |= 0x100;
    w->unk1B8 = NULL;
    w->unk30 = -0x280;
    w->unk2E = w->unk34 >> 1;
    w->unk34 = 0;
    ScreenShake_Start(4);
    w->unk62 = 0x44;
    func_ov000_0232dedc(w, 0, 0, 0, -0x200, 0);
    func_ov000_0232dedc(w, 0, 0, 0, -0x200, 1);
    func_ov000_0232e48c(w, 0x14, 0);
    func_0201c070(w->unk1DC, 0);
    Sound_PlaySeqArcEx(0, 0, 0x64);
}
