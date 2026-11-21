#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0xF4A - 0x20C];
    s16 unkF4A;
    u8 padF4C[0xF54 - 0xF4C];
    s32 unkF54;
    s32 unkF58;
    s32 unkF5C;
    s16 unkF60;
    u8 padF62[2];
    s16 unkF64;
} UnkStruct_Boss;

void Boss6_UpdateMoveSpeed(UnkStruct_Boss *w) {
    w->unkF4A += w->unkF60;
    if (w->unkF60 >= 0) {
        if (w->unkF64 < w->unkF4A)
            w->unkF4A = w->unkF64;
    } else {
        if (w->unkF64 > w->unkF4A)
            w->unkF4A = w->unkF64;
    }
    w->unkF58 += w->unkF54;
    if (w->unkF54 >= 0) {
        if (w->unkF58 > w->unkF5C)
            w->unkF58 = w->unkF5C;
    } else {
        if (w->unkF58 < w->unkF5C)
            w->unkF58 = w->unkF5C;
    }
}
