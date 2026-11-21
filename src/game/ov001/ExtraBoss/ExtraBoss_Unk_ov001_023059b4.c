#include <nitro/types.h>

typedef struct UnkStruct_023059b4 {
    u8 pad0[0x1b8];
    u8 o1B8[0x48];
    u8 o200[0x48];
    u8 o248[0x48];
    u8 o290[0x48];
    u8 pad2D8[0x5e6 - 0x2d8];
    s16 t;
    s32 unk5E8;
} UnkStruct_023059b4;

typedef struct UnkStruct_022b4574 {
    u8 pad0[8];
    void (*func)(void);
} UnkStruct_022b4574;

extern UnkStruct_023059b4 *data_ov001_0231f5a8;
extern UnkStruct_022b4574 *gTaskCurrent;

void ExtraBoss_State_Unk_ov001_0230587c(void);
void AnimatorSprite_DrawRotScale(void *p, s32 x, s32 y, s32 z);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void ExtraBoss_Unk_ov001_023059b4(void) {
    s32 i;
    s32 s;
    s32 t;

    data_ov001_0231f5a8->t += 0x199;
    if (data_ov001_0231f5a8->t >= 0x1000) {
        gTaskCurrent->func = ExtraBoss_State_Unk_ov001_0230587c;
        data_ov001_0231f5a8->unk5E8 = 0x10;
        data_ov001_0231f5a8->t = 0x1000;
    }
    i = 3;
    s = 0x1000;
    t = data_ov001_0231f5a8->t;
    do {
        s = FX_Mul(s - 0x2000, t) + 0x2000;
    } while (i-- != 0);
    AnimatorSprite_DrawRotScale(data_ov001_0231f5a8->o1B8, s, s, 0);
    AnimatorSprite_DrawRotScale(data_ov001_0231f5a8->o200, s, s, 0);
    AnimatorSprite_DrawRotScale(data_ov001_0231f5a8->o248, s, s, 0);
    AnimatorSprite_DrawRotScale(data_ov001_0231f5a8->o290, s, s, 0);
}
