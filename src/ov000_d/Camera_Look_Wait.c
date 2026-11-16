#include <nitro/types.h>

typedef struct {
    u8 pad0[0x328];
    s16 unk328;
    u8 pad32A[0x420 - 0x32A];
    u16 unk420;
} UnkStruct_023389f8_Ply;

typedef struct {
    u8 pad0[0x3A];
    s8 unk3A;
    u8 pad3B[0xC8 - 0x3B];
    u32 unkC8;
} UnkStruct_023389f8_Cam;

typedef struct {
    u32 unk0;
    u8 pad4[2];
    s16 unk6;
    s32 unk8;
    void (*unkC)(void);
} UnkStruct_023389f8;

extern UnkStruct_023389f8_Cam gCamera;
extern UnkStruct_023389f8_Ply *gPlayers[];

void Camera_Look_Reset(UnkStruct_023389f8 *w);
void Camera_Look_PanDown(void);
void Camera_Look_PanUp(void);

void Camera_Look_Wait(UnkStruct_023389f8 *w) {
    UnkStruct_023389f8_Cam *c = &gCamera;
    UnkStruct_023389f8_Ply *p;
    u16 fl;

    if (c->unkC8 & 0x40000) {
        Camera_Look_Reset(w);
        return;
    }
    p = gPlayers[c->unk3A];
    fl = p->unk420;
    if (fl & 1) {
        Camera_Look_Reset(w);
        return;
    }
    if ((w->unk0 & 2) && (!(fl & 0x40) || (p->unk328 != 0x21 && p->unk328 != 0x22))) {
        Camera_Look_Reset(w);
        return;
    }
    if ((w->unk0 & 4) && (!(fl & 0x80) || (p->unk328 != 3 && p->unk328 != 4))) {
        Camera_Look_Reset(w);
        return;
    }
    w->unk6++;
    if (w->unk6 < 0x3c) {
        return;
    }
    w->unk6 = 0;
    if (w->unk0 & 2) {
        gCamera.unkC8 |= 0x10000;
        w->unk8 = 0;
        w->unkC = Camera_Look_PanUp;
    } else {
        gCamera.unkC8 |= 0x10000;
        w->unk8 = 0;
        w->unkC = Camera_Look_PanDown;
    }
}
