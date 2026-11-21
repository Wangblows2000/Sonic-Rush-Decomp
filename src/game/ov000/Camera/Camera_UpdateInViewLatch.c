#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x34 - 4];
    u32 unk34;
    u8 pad38[0x64 - 0x38];
} UnkPlayer_023423b8;

extern UnkPlayer_023423b8 gCamera[];

void Camera_UpdateInViewLatch(void) {
    UnkPlayer_023423b8 *p;
    u8 i;

    if (gCamera[2].flags & 1) {
        p = gCamera;
        if (!(p->unk34 & 0x40) && (p->unk34 & 0x20))
            p->unk34 |= 0x40;
        else if ((p->unk34 & 0x40) && !(p->unk34 & 0x20))
            p->unk34 &= ~0x40;
        p->unk34 &= ~0x20;
        return;
    }
    for (i = 0; i < 2; i++) {
        if (!(gCamera[i].unk34 & 0x40) && (gCamera[i].unk34 & 0x20))
            gCamera[i].unk34 |= 0x40;
        else if ((gCamera[i].unk34 & 0x40) && !(gCamera[i].unk34 & 0x20))
            gCamera[i].unk34 &= ~0x40;
        gCamera[i].unk34 &= ~0x20;
    }
}
