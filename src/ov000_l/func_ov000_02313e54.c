#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x34];
    s16 unk34;
} UnkView_02313e54;

void func_ov000_02313cfc(void);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);

void func_ov000_02313e54(UnkStruct_StageObj *w, s32 a) {
    w->unk68 = func_ov000_02313cfc;
    w->unk58 |= 0x40;
    w->unk58 &= ~0x2100;
    w->unk58 &= ~0x200;
    ((UnkView_02313e54 *)w)->unk34 = -0x800;
    if (w->unk5C & 1)
        ((UnkView_02313e54 *)w)->unk34 = 0x800;
    w->unk204 = a;
    Sound_PlaySeqArcEx(w->unk1DC, 0, 99);
}
