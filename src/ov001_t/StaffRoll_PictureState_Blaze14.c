#include <nitro/types.h>

typedef struct {
    u8 pad0[0xbdc];
    s32 unkbdc;
    u8 padbe0[0xcd4 - 0xbe0];
    s32 unkcd4;
    s32 unkcd8;
    u8 padcdc[0xcec - 0xcdc];
    s32 unkcec;
    s32 unkcf0;
} UnkStruct_022d05f8;

void func_ov001_022d0220(UnkStruct_022d05f8 *p);
void StaffRoll_PictureState_Blaze15(UnkStruct_022d05f8 *p);
void StaffRoll_SetPictureState(UnkStruct_022d05f8 *p, void (*fn)(UnkStruct_022d05f8 *));

#pragma thumb on
void StaffRoll_PictureState_Blaze14(UnkStruct_022d05f8 *p) {
    func_ov001_022d0220(p);
    if (p->unkcd4 > 0x400) {
        p->unkcd4 = 0x400;
        p->unkcec = 0;
    }
    if (p->unkbdc > 0xb4) {
        p->unkcd8 = -0x400;
        p->unkcf0 = 0;
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze15);
    }
}
#pragma thumb reset
