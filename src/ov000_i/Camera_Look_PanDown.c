#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x20 - 4];
    s16 unk20;
    u8 pad22[0x3a - 0x22];
    s8 unk3A;
    u8 pad3B[0x64 - 0x3b];
} UnkPlayer_0233887c;

typedef struct {
    u8 pad0[0x328];
    s16 unk328;
    u8 pad32A[0x420 - 0x32a];
    u16 unk420;
} UnkObj_0233887c;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    void *unkC;
} UnkStruct_0233887c;

extern UnkPlayer_0233887c gCamera[];
extern UnkObj_0233887c *gPlayers[];
void Camera_Look_ReturnFromDown(void);

void Camera_Look_PanDown(UnkStruct_0233887c *w) {
    UnkPlayer_0233887c *p = gCamera;
    UnkObj_0233887c *o;

    if (p[2].flags & 0x40000) {
        w->unk8 = 0;
        w->unkC = Camera_Look_ReturnFromDown;
        p[2].flags |= 0x20000;
        return;
    }
    o = gPlayers[p->unk3A];
    if (!(o->unk420 & 0x80) || (o->unk328 != 3 && o->unk328 != 4)) {
        w->unk8 = 0;
        w->unkC = Camera_Look_ReturnFromDown;
        gCamera[2].flags |= 0x20000;
        return;
    }
    w->unk8 = (p->unk20 << 10) >> 8;
}
