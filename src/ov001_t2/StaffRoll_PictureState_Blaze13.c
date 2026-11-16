#include <nitro/types.h>

typedef struct {
    u8 pad0[0xcc0];
    s32 unkcc0;
    u8 padcc4[0xcd4 - 0xcc4];
    s32 unkcd4;
    s32 unkcd8;
    u8 padcdc[0xcec - 0xcdc];
    s32 unkcec;
    s32 unkcf0;
} UnkStruct_022d0658;

void StaffRoll_AdvancePictureAnim(UnkStruct_022d0658 *p);
void StaffRoll_SetPictureState(UnkStruct_022d0658 *p, void (*fn)(UnkStruct_022d0658 *));
void StaffRoll_PictureState_Blaze14(UnkStruct_022d0658 *p);

#pragma thumb on
void StaffRoll_PictureState_Blaze13(UnkStruct_022d0658 *p) {
    StaffRoll_AdvancePictureAnim(p);
    if (p->unkcc0 > 0x50000) {
        p->unkcc0 = 0x50000;
        p->unkcd8 = 0;
        p->unkcf0 = 0;
    }
    if (p->unkcd4 + p->unkcec > 0) {
        p->unkcd4 = 0;
        p->unkcec = p->unkcec / 4;
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze14);
    }
}
#pragma thumb reset
