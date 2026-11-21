#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x9D5 - 4];
    u8 unk9D5;
} UnkRingMgr_0235aea8;

typedef struct {
    u8 pad0[0x10];
    UnkRingMgr_0235aea8 *unk10;
} UnkTask_0235aea8;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x54 - 0x18];
    u32 unk54;
    u8 pad58[0x330 - 0x58];
    u32 unk330;
    u8 pad334[0x3AE - 0x334];
    s16 unk3AE;
} UnkStruct_0235aea8;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_0235aea8_4680;

extern s16 data_0206e470[];
extern UnkTask_0235aea8 *gRingManagerTask;
extern UnkStruct_0235aea8_4680 gSaveData;

#define IDX_0235aea8(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_0235aea8(a) data_0206e470[(u32)(IDX_0235aea8(a) << 1)]
#define COS_0235aea8(a) data_0206e470[(u32)((IDX_0235aea8(a) << 1) + 1)]

static inline fx32 Sin_0235aea8(u16 a) {
    return SIN_0235aea8(a);
}

static inline fx32 Cos_0235aea8(u16 a) {
    return COS_0235aea8(a);
}

void RingManager_SpawnRing(s32 x, s32 y, s16 vx, s16 vy, u16 flags);

void RingManager_ScatterRings(UnkStruct_0235aea8 *w) {
    UnkRingMgr_0235aea8 *mgr;
    u8 i;
    u8 n;
    u8 cnt;
    s16 vx;
    s16 vy;
    s32 ang;
    u16 flags;
    s32 sh;

    flags = 0x1000;
    n = w->unk3AE;
    ang = 0x488;

    if (gRingManagerTask == NULL) {
        return;
    }
    mgr = gRingManagerTask->unk10;
    w->unk3AE = 0;
    if (n > 0x40) {
        n = 0x40;
    }
    mgr->unk0 |= 2;
    if ((w->unk54 & 1) && !(w->unk330 & 0x40000)) {
        flags |= 0x10;
    }
    i = 0;
    cnt = 0;
    ang += mgr->unk9D5 << 8;
    for (; i < 0x40; i++) {
        if (ang >= 0) {
            sh = ang >> 8;
            if (sh >= 6) {
                sh = 9 - sh;
            }
            vx = Sin_0235aea8((ang & 0xff) << 8) >> sh;
            vy = Cos_0235aea8((ang & 0xff) << 8) >> sh;
            vx -= vx / 4;
            vy -= vy / 4;
            ang = (ang + 0x10) | 0x80;
        }
        RingManager_SpawnRing(w->x, w->y, vx, vy, flags);
        ang = -ang;
        vx = -vx;
        cnt++;
        if (cnt >= n) {
            if (gSaveData.unk8 & 1) {
                if (mgr->unk9D5 < 3) {
                    mgr->unk9D5++;
                }
            } else {
                if (mgr->unk9D5 < 4) {
                    mgr->unk9D5++;
                }
            }
            return;
        }
    }
}
