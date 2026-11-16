#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s16 unk8;
    u8 padA[0xC - 0xA];
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u16 unk12;
    void *unk14;
    u8 *unk18;
} UnkRing_0235b804;

typedef struct {
    u32 unk0;
    u8 pad4[0x1A0 - 4];
    UnkRing_0235b804 rings[0x40];
    u8 pad8A0[0x9D4 - 0x8A0];
    u8 unk9D4;
} UnkRingMgr_0235b804;

typedef struct {
    u8 pad0[0x10];
    UnkRingMgr_0235b804 *unk10;
} UnkTask_0235b804;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkStruct_0235b804;

typedef struct {
    s32 x;
    s32 y;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} UnkBox_0235b804;

extern s16 data_0206e470[];
extern UnkTask_0235b804 *gRingManagerTask;

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IDX_0235b804(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_0235b804(a) data_0206e470[(u32)(IDX_0235b804(a) << 1)]
#define COS_0235b804(a) data_0206e470[(u32)((IDX_0235b804(a) << 1) + 1)]

void ObjRect_SetBox2D(UnkBox_0235b804 *box, s32 l, s32 t, s32 r, s32 b);
BOOL ObjRect_BoxCheck(UnkBox_0235b804 *box, s32 x, s32 y, s32 l, s32 t, s32 r, s32 b);
u16 FX_Atan2(s32 y, s32 x);
s16 ObjSpd_UpSet(s16 v, s32 a, s32 b);

BOOL func_ov000_0235b804(UnkStruct_0235b804 *w) {
    UnkBox_0235b804 box = {0};
    UnkRingMgr_0235b804 *mgr;
    UnkRing_0235b804 *ring;
    BOOL result = FALSE;
    u16 count;
    u16 i;
    u16 ang;
    s32 spd;

    if (gRingManagerTask == NULL) {
        return FALSE;
    }
    mgr = gRingManagerTask->unk10;
    mgr->unk0 |= 4;
    box.x = w->x >> 8;
    box.y = w->y >> 8;
    ObjRect_SetBox2D(&box, -0x80, -0x80, 0x80, 0x80);
    count = mgr->unk9D4;
    for (i = 0; i < 0x40; i++) {
        if (count == 0) {
            break;
        }
        ring = &mgr->rings[i];
        if (ring->unk10 == 0) {
            continue;
        }
        count--;
        if ((ring->unk12 & 0x1000) && !(ring->unk12 & 0x4000) && ring->unk10 < 0xd8 &&
            ObjRect_BoxCheck(&box, ring->x >> 8, ring->y >> 8, -6, -0x10, 0x14, 0x10)) {
            result = TRUE;
            ring->unk12 |= 0x200;
            ring->unk12 |= 0x400;
            ring->unk12 |= 0x800;
            ring->unk12 |= 0x100;
            ring->unk12 |= 0x4000;
            ring->unk8 = 0;
            ring->unkC = 0x100;
            ring->unkE = 0x100;
            ring->unk14 = w;
        }
        if ((ring->unk12 & 0x4000) && ring->unk14 == w) {
            ang = FX_Atan2(box.y - (ring->y >> 8), box.x - (ring->x >> 8));
            ring->unk8 = ObjSpd_UpSet(ring->unk8, 0x10, 0x800);
            spd = ring->unk8;
            ring->x += (s16)((spd * COS_0235b804((u16)(ang + 0x1000))) >> 12);
            ring->y += (s16)((spd * SIN_0235b804((u16)(ang + 0x1000))) >> 12);
            ring->unkC -= 2;
            if (ring->unkC < 0x40) {
                ring->unkC = 0x40;
            }
            ring->unkE -= 2;
            if (ring->unkE < 0x40) {
                ring->unkE = 0x40;
            }
            if (ABS(ring->x - w->x) < 0x400 && ABS(ring->y - w->y) < 0x400) {
                ring->unk10 = 0;
            }
            result = TRUE;
        }
    }
    return result;
}
