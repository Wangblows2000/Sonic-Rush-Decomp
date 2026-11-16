#include <nitro/types.h>

typedef struct {
    u8 pad0[0xd08];
    s32 count;
} UnkStruct_022d177c;

void func_ov001_022d0070(UnkStruct_022d177c *p);
void StaffRoll_SetState(UnkStruct_022d177c *p, void (*fn)(UnkStruct_022d177c *));
void StaffRoll_State_ExtraFadeOut(UnkStruct_022d177c *p);
void StaffRoll_State_ExtraShowPicture(UnkStruct_022d177c *p);

#pragma thumb on
void StaffRoll_State_ExtraNextPicture(UnkStruct_022d177c *p) {
    s32 *c;

    func_ov001_022d0070(p);
    c = &p->count;
    (*c)++;
    if (*c >= 4) {
        StaffRoll_SetState(p, StaffRoll_State_ExtraFadeOut);
        return;
    }
    StaffRoll_SetState(p, StaffRoll_State_ExtraShowPicture);
}
#pragma thumb reset
