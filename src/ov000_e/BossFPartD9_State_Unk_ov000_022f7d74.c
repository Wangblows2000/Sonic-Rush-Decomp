#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x24];
    s32 unk24;
    s32 unk28;
} UnkStruct_Pos;

typedef struct {
    u8 pad[0x340];
    UnkStruct_Pos unk340;
} UnkStruct_Obj;

typedef struct {
    u8 pad[0x218];
    UnkStruct_Obj *unk218;
} UnkStruct_Mgr;

typedef struct UnkStruct_022f7d74 UnkStruct_022f7d74;
struct UnkStruct_022f7d74 {
    UnkStruct_StageObj base;
    u16 unk20C;
    s16 unk20E;
    s32 unk210;
    s32 unk214;
    s32 unk218;
    s16 unk21C;
};

extern UnkStruct_Mgr *data_ov000_02369b84;

void BossFPartD9_State_Unk_ov000_022f7ccc(void);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Mul64(s64 a, s64 b) {
    return (s32)((a * b + 0x800) >> 12);
}

void BossFPartD9_State_Unk_ov000_022f7d74(UnkStruct_022f7d74 *s) {
    UnkStruct_Pos *m = &data_ov000_02369b84->unk218->unk340;
    s32 v;
    s32 k;
    s32 cur;
    s32 tgt;
    s32 i;

    s->unk20C -= s->base.unk2E << 1;
    v = s->base.unk2E;
    if (v < 0)
        v = -v;
    s->unk21C += FX_Mul(v << 4, 0x20);
    if (s->unk21C >= 0x1000) {
        s->unk21C = 0x1000;
        s->base.unk58 |= 0x90;
        s->base.unk58 &= ~0x2200;
        s->base.unk68 = BossFPartD9_State_Unk_ov000_022f7ccc;
    }
    s->unk20E = (FX_Mul(s->unk21C, 0x2000000) - 0x2000000) >> 12;
    k = s->unk21C;
    cur = s->unk210;
    tgt = m->unk24 - 0x50000;
    i = 2;
    do {
        cur += Mul64(tgt - cur, k);
    } while (i-- != 0);
    s->base.unk10 = cur >> 4;
    s->base.unk14 = -(s->unk214 + FX_Mul(m->unk28 + 0x40000 - s->unk214, s->unk21C)) >> 4;
    s->base.unk18 = (s->unk218 + FX_Mul(-s->unk218, s->unk21C)) >> 4;
}
