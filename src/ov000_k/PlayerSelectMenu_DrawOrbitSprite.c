#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_02358384_S;

extern s16 data_0206e470[];

void AnimatorSprite_ProcessAnimation(void *p, s32 a, s32 b);
void AnimatorSprite_Draw(void *p);

#define IDX_8384(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_8384(a) data_0206e470[(u32)(IDX_8384(a) << 1)]
#define COS_8384(a) data_0206e470[(u32)((IDX_8384(a) << 1) + 1)]
#define FXM_8384(a, b) ((s32)(((s64)(a) * (b) + 0x800) >> 12))

static inline s32 Pos_8384(s32 v) {
    return FXM_8384(v, 0x41) + 1;
}

#pragma thumb on
void PlayerSelectMenu_DrawOrbitSprite(u8 *w, s32 n, s32 ang) {
    UnkStruct_02358384_S *s = (UnkStruct_02358384_S *)(w + 0x5c4) + (n + 5);

    s->x = (u32)(FXM_8384(SIN_8384((u16)ang), 0x41) + 1) + 0x80;
    s->y = (u32)(FXM_8384(COS_8384((u16)ang), 0x41) + 1) + 0x58;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
