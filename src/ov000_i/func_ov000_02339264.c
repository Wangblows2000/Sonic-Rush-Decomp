#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x20 - 0xc];
    s16 unk20;
    u8 pad22[0x64 - 0x22];
} UnkPlayer_02339264;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} UnkCam_02339264;

extern UnkPlayer_02339264 gCamera[];
extern UnkCam_02339264 data_ov000_0236abec;

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

void func_ov000_02339264(s32 unused, s32 idx) {
    UnkPlayer_02339264 *p = &gCamera[idx];
    u32 flags = gCamera[2].flags;
    UnkCam_02339264 *c = &data_ov000_0236abec;

    if (flags & 0x200)
        return;
    if (flags & 1) {
        if (!(flags & 2)) {
            p->unk8 = CLAMP(p->unk8, c->unk4, c->unkC - p->unk20 * 0x1d0);
            return;
        }
        p->unk8 = CLAMP(p->unk8, c->unk4 + p->unk20 * 0x110, c->unkC - p->unk20 * 0xc0);
        return;
    }
    p->unk8 = CLAMP(p->unk8, c->unk4, c->unkC - p->unk20 * 0xc0);
}
