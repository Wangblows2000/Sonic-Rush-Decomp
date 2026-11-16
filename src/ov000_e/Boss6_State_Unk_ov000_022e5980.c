#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0xF50 - 0x20C];
    s32 unkF50;
    s32 unkF54;
    s32 unkF58;
    s32 unkF5C;
    s16 unkF60;
    u8 padF62[2];
    s16 unkF64;
} UnkStruct_Boss;

extern UnkStruct_StageObj *gPlayers;

void func_ov000_022e5c1c(UnkStruct_Boss *w);
void Boss6_SelectNextAttack(s32 a);

void Boss6_State_Unk_ov000_022e5980(UnkStruct_Boss *w) {
    s32 d;

    w->unkF50--;
    func_ov000_022e5c1c(w);
    d = gPlayers->unk10 - w->base.unk10;
    if (w->unkF54 >= 0) {
        if (w->base.unk10 > 0x2aa00 || d < -0x8000) {
            w->unkF54 = -w->unkF54;
            w->unkF60 = -w->unkF60;
            w->unkF64 = -w->unkF64;
            w->unkF5C = -w->unkF5C;
        }
    } else {
        if (w->base.unk10 < 0x9600 || d > 0x8000) {
            w->unkF54 = -w->unkF54;
            w->unkF60 = -w->unkF60;
            w->unkF64 = -w->unkF64;
            w->unkF5C = -w->unkF5C;
        }
    }
    w->base.unk10 += w->unkF58;
    if (w->unkF50 > 0)
        return;
    Boss6_SelectNextAttack(0);
}
