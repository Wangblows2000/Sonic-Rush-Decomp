#include <nitro/types.h>

typedef struct {
    u8 pad0[0x3a];
    s8 unk3A;
    u8 pad3B[0x64 - 0x3b];
} UnkPlayer_02338b54;

typedef struct {
    u8 pad0[0x328];
    s16 unk328;
    u8 pad32A[0x420 - 0x32a];
    u16 unk420;
} UnkObj_02338b54;

typedef struct {
    u32 flags;
    u8 pad4[2];
    s16 unk6;
    u8 pad8[4];
    void *unkC;
} UnkStruct_02338b54;

extern UnkPlayer_02338b54 gCamera[];
extern UnkObj_02338b54 *gPlayers[];
void Camera_Look_Wait(void);

void Camera_Look_Idle(UnkStruct_02338b54 *w) {
    UnkPlayer_02338b54 *p = gCamera;
    UnkObj_02338b54 *o;

    if ((gPlayers[p->unk3A]->unk420 & 0x40) && (u16)(s16)(gPlayers[p->unk3A]->unk328 - 0x21) <= 1) {
        w->flags |= 2;
        w->unk6 = 0;
        w->unkC = Camera_Look_Wait;
    }
    o = gPlayers[p->unk3A];
    if ((o->unk420 & 0x80) && (u16)(s16)(o->unk328 - 3) <= 1) {
        w->flags |= 4;
        w->unk6 = 0;
        w->unkC = Camera_Look_Wait;
    }
}
