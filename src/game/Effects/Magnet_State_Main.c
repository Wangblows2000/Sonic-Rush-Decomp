#include <nitro/types.h>

typedef struct {
    fx32 m[3][3];
} UnkMtx33_02330848;

typedef struct {
    u8 pad0[0x24];
    UnkMtx33_02330848 mtx;
    u8 pad48[0x90 - 0x48];
    void *unk90;
    u8 pad94[0xB0 - 0x94];
} UnkMdl_02330848;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    s32 z;
    u8 pad1C[0x5C - 0x1C];
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
} UnkParent_02330848;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 x;
    s32 y;
    s32 z;
    u8 pad1C[0x54 - 0x1C];
    u32 unk54;
    u8 pad58[0x5C - 0x58];
    u16 unk5C;
    u8 pad5E[0x88 - 0x5E];
    UnkParent_02330848 *unk88;
    u8 pad8C[0x128 - 0x8C];
    UnkMdl_02330848 *unk128;
    u8 pad12C[0x4A4 - 0x12C];
    UnkMdl_02330848 mdl4A4;
    UnkMdl_02330848 mdl554;
    u8 pad604[0x75C - 0x604];
    u32 unk75C;
    u8 pad760[0x764 - 0x760];
    u16 unk764;
    u16 unk766;
    u16 unk768;
    u16 unk76A;
} UnkStruct_02330848;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02330848_4560;

extern s16 data_0206e470[];
extern s16 data_02072070[];
extern u32 gRandSeed;
extern UnkStruct_02330848_4560 gGameState;

#define IDX_02330848(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_02330848(a) data_0206e470[(u32)(IDX_02330848(a) << 1)]
#define COS_02330848(a) data_0206e470[(u32)((IDX_02330848(a) << 1) + 1)]

void NNS_G3dMdlSetMdlAlpha(void *p, s32 a, u32 b);
void MTX_Identity33_(UnkMtx33_02330848 *m);
void MTX_RotZ33_(UnkMtx33_02330848 *m, fx32 sin, fx32 cos);
void MTX_RotY33_(UnkMtx33_02330848 *m, fx32 sin, fx32 cos);
void MTX_Concat33(const UnkMtx33_02330848 *a, const UnkMtx33_02330848 *b, UnkMtx33_02330848 *ab);

static inline u16 Rand_02330848(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Magnet_State_Main(UnkStruct_02330848 *w) {
    UnkMtx33_02330848 r;
    UnkParent_02330848 *p = w->unk88;
    UnkMdl_02330848 *m = w->unk128;
    u32 alpha;
    u32 t;

    if (p != NULL) {
        w->x = w->unk88->x;
        w->y = w->unk88->y;
        w->z = w->unk88->z;
        w->unk5C &= ~0x200;
        w->unk5C |= w->unk88->unk5C & 0x200;
        if (w->unkC & 4) {
            w->unk75C |= 0x80;
        } else {
            w->unk75C &= ~0x80;
        }
        alpha = w->unk76A;
        t = gGameState.unk20 & 0xf;
        if (t >= 4) {
            alpha = 0;
        } else {
            alpha = (alpha * ((3 - t) << 6)) >> 8;
        }
        NNS_G3dMdlSetMdlAlpha(w->unk128->unk90, 4, alpha);
        MTX_Identity33_(&m->mtx);
        MTX_RotZ33_(&r, SIN_02330848(w->unk764), COS_02330848(w->unk764));
        MTX_Concat33(&m->mtx, &r, &m->mtx);
        m = &w->mdl4A4;
        MTX_Identity33_(&m->mtx);
        MTX_RotZ33_(&r, SIN_02330848(w->unk768), COS_02330848(w->unk768));
        MTX_Concat33(&m->mtx, &r, &m->mtx);
        m = &w->mdl554;
        MTX_Identity33_(&m->mtx);
        MTX_RotY33_(&r, SIN_02330848((u16)-w->unk766), COS_02330848((u16)-w->unk766));
        MTX_Concat33(&m->mtx, &r, &m->mtx);
        MTX_RotZ33_(&r, data_02072070[0], data_02072070[1]);
        MTX_Concat33(&m->mtx, &r, &m->mtx);
        if (!(p->unk32C & 0x20000000)) {
            w->unk54 |= 4;
        }
    }
    if (!(gGameState.unk20 & 7)) {
        w->unk764 += 0x2aaa;
        w->unk768 = Rand_02330848();
    }
    w->unk766 += 0x600;
    w->unk766 &= 0x7fff;
    w->unkC++;
}
