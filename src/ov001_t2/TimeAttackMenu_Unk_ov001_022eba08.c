#include <nitro/types.h>

typedef struct {
    s32 state;
    u8 pad4[0xd0 - 4];
    s32 unkd0;
} UnkWork_022eba08;

typedef struct UnkTask_022eba08 {
    u8 pad0[8];
    void (*fn)(struct UnkTask_022eba08 *);
    u8 padc[4];
    UnkWork_022eba08 *work;
} UnkTask_022eba08;

extern UnkTask_022eba08 *gTaskCurrent;

void TimeAttackMenu_IndexToGrid(UnkWork_022eba08 *w, s32 a, s32 *x, s32 *y);
void TimeAttackMenu_CreateFadeTask(s32 a, s32 b, s32 c, s32 d);
void TimeAttackMenu_Unk_ov001_022ea80c(UnkWork_022eba08 *w);
void TimeAttackMenu_Unk_ov001_022ea458(UnkWork_022eba08 *w, s32 x, s32 y);
void TimeAttackMenu_Unk_ov001_022ea7c4(UnkWork_022eba08 *w);
void TimeAttackMenu_Unk_ov001_022ea6e4(UnkWork_022eba08 *w);
void SysSound_PlaySE(s32 a);
void TimeAttackMenu_Unk_ov001_022eab78(UnkTask_022eba08 *t);

#pragma thumb on
void TimeAttackMenu_Unk_ov001_022eba08(UnkTask_022eba08 *t) {
    UnkWork_022eba08 *w;
    s32 x;
    s32 y;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->state = 0;
    TimeAttackMenu_IndexToGrid(w, w->unkd0, &x, &y);
    TimeAttackMenu_CreateFadeTask(0, 0x10, 8, 8);
    TimeAttackMenu_Unk_ov001_022ea80c(w);
    TimeAttackMenu_Unk_ov001_022ea458(w, x, y);
    TimeAttackMenu_Unk_ov001_022ea7c4(w);
    TimeAttackMenu_Unk_ov001_022ea6e4(w);
    SysSound_PlaySE(0);
    t->fn = TimeAttackMenu_Unk_ov001_022eab78;
}
#pragma thumb reset
