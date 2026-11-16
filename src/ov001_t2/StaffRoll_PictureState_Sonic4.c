#include <nitro/types.h>

typedef struct {
    u8 pad0[0xbdc];
    s32 unkbdc;
    u8 padbe0[0xcbc - 0xbe0];
    s32 unkcbc;
    s32 unkcc0;
    u8 padcc4[0xcd4 - 0xcc4];
    s32 unkcd4;
    s32 unkcd8;
} UnkStruct_022d1238;

void StaffRoll_SetPictureState(UnkStruct_022d1238 *p, void (*fn)(UnkStruct_022d1238 *));
void StaffRoll_PictureState_Sonic5(UnkStruct_022d1238 *p);

#pragma thumb on
void StaffRoll_PictureState_Sonic4(UnkStruct_022d1238 *p) {
    if (p->unkcd4 <= -0x3000) {
        p->unkcd4 = -0x3000;
    }
    if (p->unkcd8 <= -0x3000) {
        p->unkcd8 = -0x3000;
    }
    if (p->unkcbc < -0x28000) {
        p->unkcbc = -0x28000;
    }
    if (p->unkcc0 < -0x28000) {
        p->unkcc0 = -0x28000;
    }
    if (p->unkbdc > 0xb4) {
        StaffRoll_SetPictureState(p, StaffRoll_PictureState_Sonic5);
    }
}
#pragma thumb reset
