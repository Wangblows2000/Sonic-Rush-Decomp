#include <nitro/types.h>

typedef struct UnkStruct_022f8608_spr {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x44 - 8];
    u16 unk44;
    u8 pad46[2];
} UnkStruct_022f8608_spr;

typedef struct UnkStruct_022f8608 {
    u8 pad0[0x18];
    UnkStruct_022f8608_spr sprs[1];
    u8 pad60[0xe38 - 0x60];
    u32 flags;
    u8 padE3C[0xe58 - 0xe3c];
    s32 unkE58;
    s16 unkE5C;
} UnkStruct_022f8608;

typedef struct UnkStruct_022f8608_t {
    u8 pad0[8];
    void (*unk8)(void);
} UnkStruct_022f8608_t;

extern UnkStruct_022f8608 *data_ov001_0231f084;
extern UnkStruct_022f8608_t *gTaskCurrent;
extern u16 data_ov001_02316adc[];

void ExtraAct_State_Unk_ov001_022f8330(void);
void ExtraAct_State_Unk_ov001_022f8508(void);
void AnimatorSprite_Draw(void *p);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void ExtraAct_Unk_ov001_022f8608(void) {
    UnkStruct_022f8608_spr *spr;
    s32 v;
    s32 acc;
    s32 i;
    u16 idx;

    if (data_ov001_0231f084->flags & 3) {
        gTaskCurrent->unk8 = ExtraAct_State_Unk_ov001_022f8330;
        ExtraAct_State_Unk_ov001_022f8330();
        data_ov001_0231f084->flags &= ~0x1000;
        data_ov001_0231f084->flags |= 0x2000;
        return;
    }
    data_ov001_0231f084->unkE5C += 0x333;
    v = data_ov001_0231f084->unkE5C;
    if (v >= 0x1000) {
        gTaskCurrent->unk8 = ExtraAct_State_Unk_ov001_022f8508;
        acc = 0xb4000;
        data_ov001_0231f084->unkE58 = 0xf;
    } else {
        i = 3;
        acc = 0xe0000;
        do {
            acc += FX_Mul(0xb4000 - acc, v);
        } while (i-- != 0);
    }
    for (i = 0; i < 0x10; i++) {
        idx = data_ov001_02316adc[i];
        spr = &data_ov001_0231f084->sprs[idx + 0x18];
        if (idx != 0xe) {
            spr->x = 0x24;
            spr->y = acc >> 12;
            spr->unk44 = 1;
            AnimatorSprite_Draw(spr);
        }
    }
}
