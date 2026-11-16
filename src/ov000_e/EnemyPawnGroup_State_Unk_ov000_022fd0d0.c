#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_StageObj *gPlayers;

void Sound_PlaySeqArcEx(s32 p, s32 a, s32 b);
void Sound_SetPositional(s32 a, UnkStruct_StageObj *w, UnkStruct_StageObj *p);
void func_ov000_022fc5b8(UnkStruct_StageObj *w, s32 type, s16 dx, s16 dy);
void func_ov000_022fe7e8(UnkStruct_StageObj *w);

void EnemyPawnGroup_State_Unk_ov000_022fd0d0(UnkStruct_StageObj *w) {
    switch (w->spr0.unkE >> 4) {
    case 0x2e:
    case 0x30:
    case 0x32:
        Sound_PlaySeqArcEx(w->unk1DC, 0, 0x79);
        Sound_SetPositional(w->unk1DC, w, gPlayers);
        func_ov000_022fc5b8(w, 0xec, 0x28,
                            ((u16)(((data_022c4560.unk20 << 16) * 0x196225 + 0x3c6ef35f) >> 16) & 3) - 0x20);
        break;
    }
    if (!(w->unk5C & 8))
        return;
    w->unk1F4 &= ~5;
    func_ov000_022fe7e8(w);
}
