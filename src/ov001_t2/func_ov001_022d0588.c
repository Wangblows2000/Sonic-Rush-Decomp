#include <nitro/types.h>

typedef struct {
    u8 pad0[0xcbc];
    s32 unkcbc;
    s32 unkcc0;
    u8 padcc4[0xcd4 - 0xcc4];
    s32 unkcd4;
    s32 unkcd8;
    u8 padcdc[0xcec - 0xcdc];
    s32 unkcec;
    s32 unkcf0;
} UnkStruct_022d0588;

void func_ov001_022d0220(UnkStruct_022d0588 *p);
void StaffRoll_SetPictureState(UnkStruct_022d0588 *p, void (*fn)(UnkStruct_022d0588 *));
void func_ov001_022d0544(UnkStruct_022d0588 *p);

#pragma thumb on
void func_ov001_022d0588(UnkStruct_022d0588 *p) {
    func_ov001_022d0220(p);
    if (p->unkcd4 > 0x400) {
        p->unkcd4 = 0x400;
        p->unkcec = 0;
    }
    if (p->unkcbc - p->unkcc0 >= 0x96000) {
        p->unkcd4 = 0;
        p->unkcec = 0;
        p->unkcd8 = 0;
        p->unkcf0 = 0;
        StaffRoll_SetPictureState(p, func_ov001_022d0544);
    }
}
#pragma thumb reset
