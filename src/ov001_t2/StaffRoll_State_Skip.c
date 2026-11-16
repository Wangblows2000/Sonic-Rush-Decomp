#include <nitro/types.h>

typedef struct {
    s32 mode;
    u8 pad4[0xbb0 - 4];
    s32 unkbb0;
    s32 unkbb4;
} UnkStruct_022d1c20;

void StaffRoll_ResetBgScroll(UnkStruct_022d1c20 *p);
void StaffRoll_DestroyCreditTask(s32 i);
void StaffRoll_SetState(UnkStruct_022d1c20 *p, void (*fn)(UnkStruct_022d1c20 *));
void StaffRoll_State_SkipSonic(UnkStruct_022d1c20 *p);
void StaffRoll_State_SkipBlaze(UnkStruct_022d1c20 *p);
void StaffRoll_State_SkipExtra(UnkStruct_022d1c20 *p);

#pragma thumb on
void StaffRoll_State_Skip(UnkStruct_022d1c20 *p) {
    s32 i;

    p->unkbb0 = 0;
    p->unkbb4 = 0;
    StaffRoll_ResetBgScroll(p);
    for (i = 0; i < 4; i++) {
        StaffRoll_DestroyCreditTask(i);
    }
    switch (p->mode) {
    case 0:
        StaffRoll_SetState(p, StaffRoll_State_SkipSonic);
        break;
    case 1:
        StaffRoll_SetState(p, StaffRoll_State_SkipBlaze);
        break;
    case 2:
        StaffRoll_SetState(p, StaffRoll_State_SkipExtra);
        break;
    }
}
#pragma thumb reset
