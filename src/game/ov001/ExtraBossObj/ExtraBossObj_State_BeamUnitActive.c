#include <nitro/types.h>

typedef struct UnkStruct_BeamUnit {
    u8 pad0[0x14];
    u32 flags;
    s32 x;
    s32 y;
    u8 pad20[0x50 - 0x20];
    s32 velX;
    s32 velY;
    u8 pad58[0x74 - 0x58];
    void (*state)(void);
    u8 pad78[0xb8 - 0x78];
    u32 unkB8;
    u8 padBC[0x1c8 - 0xbc];
    s32 kind;
    s32 timer1CC;
    s32 unk1D0;
    s32 timer1D4;
    s32 timer1D8;
} UnkStruct_BeamUnit;

typedef struct {
    u8 pad0[0x40];
    s32 unk40;
} UnkExtraBossWork;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkGameState;

typedef struct {
    void *res;
    u8 pad4[0x18 - 4];
} UnkFxRes;

extern u8 *data_ov001_0231f094;
extern UnkFxRes gExtraBossFxRes[];
extern s16 data_0206e470[];
extern u16 data_ov001_0231d5d8[][5];
extern UnkGameState gGameState;
extern u32 gRandSeed;

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

UnkStruct_BeamUnit *ExtraBoss_GetPlayer(void);
UnkExtraBossWork *ExtraBoss_GetWork(void);
void ExtraBoss_SpawnBombFx(s32 a, s32 b, s32 c, s32 d, BOOL flag);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);
s32 FX_Sqrt(s32 x);
u16 FX_Atan2(s32 y, s32 x);
u8 *NNS_G3dGetMdlSet(void *p);
void NNS_G3dMdlSetMdlDiffAll(void *s, u32 v);
void NNS_G3dMdlSetMdlAmbAll(void *s, u32 v);
void NNS_G3dMdlSetMdlEmiAll(void *s, u32 v);
void ExtraBossObj_State_BeamUnitCharge(void);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

static inline BOOL IsSpecialMode(void) {
    BOOL r = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        r = FALSE;
    return r;
}

void ExtraBossObj_State_BeamUnitActive(UnkStruct_BeamUnit *p) {
    u8 *h;
    u16 ang;
    UnkStruct_BeamUnit *pl;
    s32 dx;
    s32 idx;
    u8 *r;
    s32 dist;
    s32 adj;
    s32 diff;
    s32 dy;

    pl = ExtraBoss_GetPlayer();
    p->timer1D8--;
    if (p->timer1D8 > 0) {
        if (p->timer1D8 & 2)
            p->flags |= 0x20000;
        else
            p->flags &= ~0x20000;
    } else if (p->timer1D8 == 0) {
        p->flags &= ~0x20000;
        p->unkB8 &= ~0x200;
    }
    p->timer1CC--;
    p->timer1D4--;
    if (p->timer1D4 <= 0) {
        p->flags |= 8;
        if (p->kind == 8)
            *data_ov001_0231f094 &= ~1;
        else
            *data_ov001_0231f094 &= ~2;
        ExtraBoss_SpawnBombFx(0, p->x, p->y, 0, 0);
        Sound_PlaySeqArc(0, 0, 0xac);
        return;
    }
    if (p->timer1CC <= 0 && p->timer1D4 > 0x78) {
        p->state = ExtraBossObj_State_BeamUnitCharge;
        p->velY = 0;
        p->velX = p->velY;
        p->timer1CC = 0x1e;
        if (ExtraBoss_GetWork()->unk40 == 4)
            p->unk1D0 = (Rand() & 1) ? 1 : 3;
        else
            p->unk1D0 = 1;
    } else {
        dx = p->x - pl->x;
        dy = p->y - pl->y;
        dist = FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
        adj = 0;
        if (dist > 0x30000) {
            adj = -0x1e00;
            if (0x30000 - dist > adj)
                adj = 0x30000 - dist;
        } else if (dist < 0x30000) {
            adj = 0x1e00;
            if (0x30000 - dist < 0x1e00)
                adj = 0x30000 - dist;
        }
        dist += adj;
        ang = FX_Atan2(dy, dx);
        if (IsSpecialMode())
            diff = 1;
        else
            diff = gGameState.unk14;
        idx = (s32)((u16)(ang + data_ov001_0231d5d8[diff][ExtraBoss_GetWork()->unk40]) & 0xffff) >> 4;
        {
            s32 vx = FX_Mul(dist, COS_IDX(idx));
            s32 vy;
            p->velX = vx + pl->x - p->x;
            vy = FX_Mul(dist, SIN_IDX(idx));
            p->velY = vy + pl->y - p->y;
        }
    }
    if (p->timer1D4 > 0x78)
        return;
    if (p->timer1D4 & 3)
        return;
    if (p->timer1D4 & 4) {
        r = NNS_G3dGetMdlSet(gExtraBossFxRes[p->kind].res);
        h = r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc);
        NNS_G3dMdlSetMdlDiffAll(h, 0x7fff);
        NNS_G3dMdlSetMdlAmbAll(h, 0);
        NNS_G3dMdlSetMdlEmiAll(h, 0);
    } else {
        r = NNS_G3dGetMdlSet(gExtraBossFxRes[p->kind].res);
        h = r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc);
        NNS_G3dMdlSetMdlDiffAll(h, 0x85f);
        NNS_G3dMdlSetMdlAmbAll(h, 0x85f);
        NNS_G3dMdlSetMdlEmiAll(h, 0x85f);
    }
}
