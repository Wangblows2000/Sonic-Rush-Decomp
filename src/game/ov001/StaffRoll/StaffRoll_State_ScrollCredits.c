#include <nitro/types.h>

typedef struct {
    u8 pad0[0xbbc];
    s32 timer;
    u8 padbc0[4];
    s32 end;
    void *objs[4];
} UnkStruct_022d2060;

extern s32 data_ov001_02316108[];

void *StaffRoll_CreateCreditTask(s32 i);
void StaffRoll_ResetBgScroll(UnkStruct_022d2060 *p);
void StaffRoll_SetState(UnkStruct_022d2060 *p, void (*fn)(UnkStruct_022d2060 *));
void StaffRoll_State_StartFinalCredit(UnkStruct_022d2060 *p);

#pragma thumb on
void StaffRoll_State_ScrollCredits(UnkStruct_022d2060 *p) {
    s32 prev = p->timer++;
    s32 i;

    for (i = 0; i < 4; i++) {
        s32 t = data_ov001_02316108[i] - 0x1d0;
        if (t > prev && t <= p->timer && p->objs[i] == NULL) {
            p->objs[i] = StaffRoll_CreateCreditTask(i);
        }
    }
    if (p->timer >= p->end) {
        StaffRoll_ResetBgScroll(p);
        StaffRoll_SetState(p, StaffRoll_State_StartFinalCredit);
    }
}
#pragma thumb reset
