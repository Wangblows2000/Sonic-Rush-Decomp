#include <nitro/types.h>

typedef struct UnkStruct_022cc1f0 {
    u8 pad[0x14];
    s32 unk14;
    s32 unk18;
} UnkStruct_022cc1f0;

BOOL Boss1Neck_Unk_ov000_022cc1f0(s32 x, UnkStruct_022cc1f0 *s) {
    BOOL r = FALSE;
    s32 d = x - s->unk18;
    if (d < 0)
        d = -d;
    if (d < 0x10000) {
        d = 0x20000 - s->unk14;
        if (d < 0)
            d = -d;
        if (d < 0xb400)
            r = TRUE;
    }
    return r;
}
