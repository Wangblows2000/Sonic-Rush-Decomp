#include <nitro/types.h>

typedef struct UnkStruct_022f8b04_spr {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x44 - 0x30];
    u16 unk44;
} UnkStruct_022f8b04_spr;

typedef struct UnkStruct_022f8b04 {
    u8 pad0[0xac8];
    UnkStruct_022f8b04_spr spr;
    u8 padB10[0xe38 - 0xb10];
    u32 flags;
    u8 padE3C[0xe48 - 0xe3c];
    s16 unkE48;
} UnkStruct_022f8b04;

typedef struct UnkStruct_022f8b04_t {
    u8 pad0[8];
    void (*unk8)(void);
} UnkStruct_022f8b04_t;

extern UnkStruct_022f8b04 *data_ov001_0231f084;
extern UnkStruct_022f8b04_t *gTaskCurrent;

void func_ov001_022f8a98(void);
void AnimatorSprite_Draw(void *p);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_022f8b04(void) {
    UnkStruct_022f8b04_spr *spr;
    s32 v;
    s32 acc;
    s32 i;
    s16 y;

    if (data_ov001_0231f084->flags & 3) {
        gTaskCurrent->unk8 = func_ov001_022f8a98;
        data_ov001_0231f084->flags &= ~0x40;
        data_ov001_0231f084->flags |= 0x80;
        return;
    }
    data_ov001_0231f084->unkE48 += 0x100;
    if (data_ov001_0231f084->unkE48 >= 0x1000) {
        data_ov001_0231f084->unkE48 = 0x1000;
        gTaskCurrent->unk8 = func_ov001_022f8a98;
        data_ov001_0231f084->flags |= 0x100;
    }
    i = 5;
    acc = 0;
    v = data_ov001_0231f084->unkE48;
    do {
        acc += FX_Mul(0x10 - acc, v);
    } while (i-- != 0);
    spr = &data_ov001_0231f084->spr;
    y = acc;
    spr->x = 0;
    spr->y = y - 0x10;
    spr->unk44 = 0;
    spr->flags &= ~0x100;
    AnimatorSprite_Draw(spr);
    spr->y = 0xd0 - y;
    spr->unk44 = 1;
    spr->flags |= 0x100;
    AnimatorSprite_Draw(spr);
}
