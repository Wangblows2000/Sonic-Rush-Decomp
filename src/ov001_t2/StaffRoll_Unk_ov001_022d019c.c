#include <nitro/types.h>

typedef struct {
    u8 pad0[0xcbc];
    s32 unkcbc;
    u8 padcc0[4];
    s32 unkcc4;
    u8 padcc8[0xcd0 - 0xcc8];
    s32 unkcd0;
    u8 padcd4[0xcdc - 0xcd4];
    s32 unkcdc;
    u8 padce0[0xd04 - 0xce0];
    s32 unkd04;
} UnkStruct_022d019c;

#pragma thumb on
void StaffRoll_Unk_ov001_022d019c(UnkStruct_022d019c *p) {
    s32 d;

    if (p->unkd04 == 1) {
        d = p->unkcbc + 0x14000 - p->unkcc4;
        if (d > 0) {
            p->unkcdc = 0x800;
        } else if (d < -0x14000) {
            p->unkcdc = -0xc00;
        } else {
            p->unkcdc *= 0.9f;
        }
        p->unkcd0 = 0x46000;
    }
}
#pragma thumb reset
