#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0xF54 - 0x20C];
    s32 unkF54;
    s32 unkF58;
    s32 unkF5C;
    s16 unkF60;
    u8 padF62[2];
    s16 unkF64;
} UnkStruct_022e51a4;

extern UnkStruct_StageObj *gPlayers;

void Boss6_Unk_ov000_022e5c1c(UnkStruct_022e51a4 *s);
void Boss6_State_Unk_ov000_022e5078(void);

void Boss6_State_Unk_ov000_022e51a4(UnkStruct_022e51a4 *s) {
    s32 px = gPlayers->unk10;

    Boss6_Unk_ov000_022e5c1c(s);
    if (s->unkF54 >= 0) {
        if (s->base.unk10 > 0x2aa00) {
            s->unkF54 = -s->unkF54;
            s->unkF60 = -s->unkF60;
            s->unkF64 = -s->unkF64;
            s->unkF5C = -s->unkF5C;
        }
    } else if (s->base.unk10 < 0x9600) {
        s->unkF54 = -s->unkF54;
        s->unkF60 = -s->unkF60;
        s->unkF64 = -s->unkF64;
        s->unkF5C = -s->unkF5C;
    }
    s->base.unk10 += s->unkF58;
    if (px >= s->base.unk10 + 0x3200)
        return;
    if (px <= s->base.unk10 - 0x3200)
        return;
    s->base.unk68 = Boss6_State_Unk_ov000_022e5078;
    if (s->unkF54 >= 0) {
        if (s->unkF58 > 0) {
            s->unkF54 = -s->unkF54;
            s->unkF60 = -s->unkF60;
        }
    } else if (s->unkF58 < 0) {
        s->unkF54 = -s->unkF54;
        s->unkF60 = -s->unkF60;
    }
    s->unkF64 = 0;
    s->unkF5C = 0;
}
