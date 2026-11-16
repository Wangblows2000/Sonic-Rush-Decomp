#include <nitro/types.h>

typedef struct UnkStruct_022fc09c_k {
    void *res;
    u8 pad4[0x18 - 4];
} UnkStruct_022fc09c_k;

extern u8 *data_ov001_0231f094;
extern UnkStruct_022fc09c_k data_ov001_0231f098[];
extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

u8 *ExtraBossPlayer_Unk_ov001_022fd428(s32 kind, void *parent, s32 x, s32 y, s32 z);
u8 *NNS_G3dGetMdlSet(void *p);
void NNS_G3dMdlSetMdlDiffAll(void *s, u32 v);
void NNS_G3dMdlSetMdlAmbAll(void *s, u32 v);
void NNS_G3dMdlSetMdlEmiAll(void *s, u32 v);
void *ExtraBoss_GetPlayer(void);
void ExtraBossObj_State_Unk_ov001_022fbfcc(void);
void ExtraBoss_Unk_ov001_022fb974(void);
void ExtraBoss_Unk_ov001_022fb918(void);
void ExtraBoss_Unk_ov001_022fe8d4(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void ExtraBoss_Unk_ov001_022fc09c(void *parent, s32 x, s32 y, s32 z, u16 ang) {
    s32 kind;
    u8 *t;
    u8 *r;
    u8 *h;
    s32 idx;
    s32 s;
    s32 c;

    if (*data_ov001_0231f094 & 1) {
        *data_ov001_0231f094 |= 2;
        kind = 9;
    } else {
        *data_ov001_0231f094 |= 1;
        kind = 8;
    }
    t = ExtraBossPlayer_Unk_ov001_022fd428(kind, parent, x, y, z);
    if (t == NULL)
        return;
    r = NNS_G3dGetMdlSet(data_ov001_0231f098[kind].res);
    h = r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc);
    NNS_G3dMdlSetMdlDiffAll(h, 0x7fff);
    NNS_G3dMdlSetMdlAmbAll(h, 0);
    NNS_G3dMdlSetMdlEmiAll(h, 0);
    *(void **)(t + 0x80) = ExtraBoss_GetPlayer();
    *(void **)(t + 0x74) = ExtraBossObj_State_Unk_ov001_022fbfcc;
    *(s32 *)(t + 0x18) = x;
    *(s32 *)(t + 0x1c) = y;
    *(s32 *)(t + 0x20) = z;
    idx = (s32)(ang & 0xffff) >> 4;
    *(s32 *)(t + 0x50) = FX_Mul(COS_IDX(idx), 0xa000);
    *(s32 *)(t + 0x54) = FX_Mul(SIN_IDX(idx), 0xa000);
    *(s32 *)(t + 0x5c) = FX_Mul(COS_IDX(idx), -0xa00);
    *(s32 *)(t + 0x60) = FX_Mul(SIN_IDX(idx), -0xa00);
    ExtraBoss_Unk_ov001_022fe8d4(t + 0xb0, t, 2, 0, 0xa000, 0xa000, 0xa000, 0, 0, 0, 0);
    *(void **)(t + 0xb4) = ExtraBoss_Unk_ov001_022fb974;
    *(u32 *)(t + 0xb8) |= 0x2200;
    *(u8 **)(t + 0x9c) = t + 0xb0;
    ExtraBoss_Unk_ov001_022fe8d4(t + 0xe8, t, 4, 1, 0xa000, 0xa000, 0xa000, 0, 0, 0, 0);
    *(void **)(t + 0xec) = ExtraBoss_Unk_ov001_022fb918;
    *(u32 *)(t + 0xf0) |= 0x2000;
    *(s32 *)(t + 0x1d8) = 0x78;
    Sound_PlaySeqArc(0, 0, 0xaa);
}
