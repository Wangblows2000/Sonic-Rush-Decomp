#include <nitro/types.h>

typedef struct {
    u32 flags;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad14[0x1c - 0x14];
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[0x64 - 0x1e];
} UnkPlayer_023391dc;

extern UnkPlayer_023391dc gCamera[];

void Camera_ClampXAroundTarget(s32 unused, s32 idx) {
    UnkPlayer_023391dc *p = &gCamera[idx];

    if (gCamera[2].flags & 0x200)
        return;
    if (!(p->flags & 0x100000))
        return;
    if (p->unk4 > (p->unk1C << 8) + p->unkC) {
        p->unk4 = (p->unk1C << 8) + p->unkC;
        return;
    }
    if (p->unk4 < p->unkC - (p->unk1C << 8))
        p->unk4 = p->unkC - (p->unk1C << 8);
    else
        p->flags |= 0x400000;
}
