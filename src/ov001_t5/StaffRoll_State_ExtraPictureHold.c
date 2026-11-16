#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 timer;
    u8 pad18[0xd08 - 0x18];
    s32 count;
} UnkStruct_022d1840;

void func_ov001_022d0070(UnkStruct_022d1840 *p);
void StaffRoll_SetState(UnkStruct_022d1840 *p, void (*fn)(UnkStruct_022d1840 *));
void StaffRoll_State_ExtraFadeOut(UnkStruct_022d1840 *p);
void StaffRoll_State_ExtraPictureFadeOut(UnkStruct_022d1840 *p);

#pragma thumb on
void StaffRoll_State_ExtraPictureHold(UnkStruct_022d1840 *p) {
    func_ov001_022d0070(p);
    if (p->timer > 0x12c) {
        if (p->count + 1 >= 4) {
            StaffRoll_SetState(p, StaffRoll_State_ExtraFadeOut);
            return;
        }
        StaffRoll_SetState(p, StaffRoll_State_ExtraPictureFadeOut);
    }
}
#pragma thumb reset
