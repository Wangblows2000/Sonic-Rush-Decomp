#include <nitro/types.h>

typedef struct {
    u8 pad0[0xbdc];
    s32 unkbdc;
    u8 padbe0[0xc58 - 0xbe0];
    u32 flags;
    u8 padc5c[0xcd4 - 0xc5c];
    s32 unkcd4;
    u8 padcd8[0xcec - 0xcd8];
    s32 unkcec;
} UnkStruct_022d0760;

void StaffRoll_AdvancePictureAnim(UnkStruct_022d0760 *p);
void StaffRoll_SetPictureState(UnkStruct_022d0760 *p, void (*fn)(UnkStruct_022d0760 *));
void StaffRoll_PictureState_Blaze12(UnkStruct_022d0760 *p);

#pragma thumb on
void StaffRoll_PictureState_Blaze11(UnkStruct_022d0760 *p) {
    StaffRoll_AdvancePictureAnim(p);
    if (p->unkcd4 + p->unkcec > 0) {
        p->unkcd4 = 0;
        p->unkcec = p->unkcec / 4;
    }
    if (p->unkbdc > 0x14) {
        p->flags &= ~4;
        if (p->flags & 2) {
            StaffRoll_SetPictureState(p, StaffRoll_PictureState_Blaze12);
        }
    }
}
#pragma thumb reset
