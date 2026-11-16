#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x2E - 0x14];
    s16 unk2E;
} UnkStruct_022f6a80;

BOOL BossFPartDE_Unk_ov000_022f6a80(UnkStruct_022f6a80 *s) {
    s16 a;

    if (s->unk10 < 0x8000) {
        a = s->unk2E;
        if (a < 0x800) {
            if (0x800 - a > 0x40) {
                s->unk2E = a + 0x40;
            } else {
                s->unk2E = 0x800;
            }
        } else if (a > 0x800) {
            if (a - 0x800 > 0x40) {
                s->unk2E = a - 0x40;
            } else {
                s->unk2E = 0x800;
            }
        }
        return FALSE;
    }
    if (s->unk10 > 0x2c000) {
        a = s->unk2E;
        if (a < -0x800) {
            if (-0x800 - a > 0x40) {
                s->unk2E = a + 0x40;
            } else {
                s->unk2E = -0x800;
            }
        } else if (a > -0x800) {
            if (a + 0x800 > 0x40) {
                s->unk2E = a - 0x40;
            } else {
                s->unk2E = -0x800;
            }
        }
        return FALSE;
    }
    a = s->unk2E;
    if (a < 0) {
        if (-a > 0x80) {
            s->unk2E = a + 0x80;
        } else {
            s->unk2E = 0;
        }
    } else if (a > 0) {
        if (a > 0x80) {
            s->unk2E = a - 0x80;
        } else {
            s->unk2E = 0;
        }
    }
    return s->unk2E == 0;
}
