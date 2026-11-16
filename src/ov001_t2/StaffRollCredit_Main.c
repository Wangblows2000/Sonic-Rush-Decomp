#include <nitro/types.h>

typedef struct {
    u8 pad0[0x7c];
} UnkObj_022d0094;

typedef struct {
    u8 pad0[0x9c0];
    UnkObj_022d0094 objs[4];
} UnkMgr_022d0094;

typedef struct {
    s32 idx;
    s32 unk4;
    s32 unk8;
} UnkWork_022d0094;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d0094 *work;
} UnkTask_022d0094;

extern UnkTask_022d0094 *gTaskCurrent;
extern UnkMgr_022d0094 *gStaffRollWork;

void StaffRoll_GetSpriteFrameSize(UnkObj_022d0094 *o, s32 a, s32 *out);
void AnimatorSpriteDS_ProcessAnimation(UnkObj_022d0094 *o, s32 a, s32 b);
void StaffRoll_SetCreditSpritePos(UnkObj_022d0094 *o, s32 a, s32 b);
void StaffRoll_DestroyCreditTask(s32 i);

#pragma thumb on
void StaffRollCredit_Main(void) {
    UnkWork_022d0094 *w = gTaskCurrent->work;
    s32 idx = w->idx;
    s32 v[2];

    StaffRoll_GetSpriteFrameSize(&gStaffRollWork->objs[idx], 0, v);
    w->unk8--;
    if (idx == 2) {
        if (w->unk8 < 16) {
            w->unk8 = 16;
        }
    } else if (idx == 3) {
        w->unk4 = 0;
        w->unk8 = 0x110;
    }
    AnimatorSpriteDS_ProcessAnimation(&gStaffRollWork->objs[idx], 0, 0);
    StaffRoll_SetCreditSpritePos(&gStaffRollWork->objs[idx], w->unk4, w->unk8);
    if (w->unk8 + v[0] < 0) {
        StaffRoll_DestroyCreditTask(idx);
    }
}
#pragma thumb reset
