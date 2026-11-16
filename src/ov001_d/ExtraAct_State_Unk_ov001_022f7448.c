#include <nitro/types.h>

typedef struct UnkStruct_022f7448_spr {
    u8 pad0[4];
    s16 x;
    s16 y;
} UnkStruct_022f7448_spr;

typedef struct UnkStruct_022f7448 {
    u8 pad0[0xb10];
    UnkStruct_022f7448_spr spr;
    u8 padB18[0xe38 - 0xb18];
    u32 flags;
    u8 padE3C[0xe9c - 0xe3c];
    s32 timer;
} UnkStruct_022f7448;

typedef struct UnkStruct_022f7448_t {
    u8 pad0[8];
    void (*unk8)(void);
} UnkStruct_022f7448_t;

extern UnkStruct_022f7448 *data_ov001_0231f084;
extern UnkStruct_022f7448_t *gTaskCurrent;
extern u32 gRandSeed;

void ExtraAct_State_Unk_ov001_022f741c(void);
s32 FX_DivS32(s32 n, s32 d);
s32 FX_ModS32(s32 n, s32 d);
void AnimatorSprite_Draw(void *p);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void ExtraAct_State_Unk_ov001_022f7448(void) {
    s32 r;
    s32 x;
    s32 y;
    UnkStruct_022f7448_spr *spr;

    if (data_ov001_0231f084->flags & 2) {
        gTaskCurrent->unk8 = ExtraAct_State_Unk_ov001_022f741c;
        ExtraAct_State_Unk_ov001_022f741c();
        data_ov001_0231f084->flags &= ~0x10000;
        data_ov001_0231f084->flags |= 0x20000;
        return;
    }
    data_ov001_0231f084->timer--;
    if (data_ov001_0231f084->timer <= 0) {
        gTaskCurrent->unk8 = ExtraAct_State_Unk_ov001_022f741c;
        data_ov001_0231f084->flags |= 2;
        data_ov001_0231f084->flags |= 0x10000;
    }
    r = FX_DivS32(0x5000, data_ov001_0231f084->timer << 4);
    if (r == 0) {
        x = 0x50000;
        y = 0x8a000;
    } else {
        x = FX_ModS32(Rand(), r) + 0x50000 - (r >> 1);
        y = FX_ModS32(Rand(), r) + 0x8a000 - (r >> 1);
    }
    spr = &data_ov001_0231f084->spr;
    spr->x = x >> 12;
    spr->y = y >> 12;
    AnimatorSprite_Draw(spr);
}
