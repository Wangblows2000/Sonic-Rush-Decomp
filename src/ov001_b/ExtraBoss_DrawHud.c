#include <nitro/types.h>

typedef struct UnkStruct_022fd744_s {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_022fd744_s;

typedef struct UnkStruct_022fd744_q {
    u8 pad0[0x3c];
    s32 unk3C;
    u8 pad40[4];
    s32 unk44;
    u8 pad48[4];
    s32 unk4C;
} UnkStruct_022fd744_q;

extern UnkStruct_022fd744_s *data_ov001_0231f324;
extern s16 data_ov001_02316e84[];

UnkStruct_022fd744_q *ExtraBoss_GetWork(void);
s32 FX_DivS32(s32 n, s32 d);
s32 FX_ModS32(s32 n, s32 d);
void AnimatorSprite_Draw(void *p);
void Sound_PlaySeqArc(u32 a, u32 b, u32 c);

#define G data_ov001_0231f324

void ExtraBoss_DrawHud(void) {
    u16 d[7];
    s32 i;
    s32 div;
    s32 n;
    s16 x;
    s32 off;
    s32 t;
    s32 r;
    s32 lives;
    UnkStruct_022fd744_s *e;
    UnkStruct_022fd744_q *q;

    n = ExtraBoss_GetWork()->unk4C;
    if (n > 999)
        n = 999;
    else if (n < 0)
        n = 0;
    off = 0;
    if (n <= 10) {
        *(s32 *)((u8 *)G + 0x798) += 1;
        if (*(s32 *)((u8 *)G + 0x798) & 8)
            off = 10;
    }
    for (i = 0, x = 0x1c, div = 100; i < 3; i++, div /= 10) {
        e = &G[off + FX_DivS32(n, div)];
        e->x = x;
        e->y = 0x12;
        AnimatorSprite_Draw(e);
        n = FX_ModS32(n, div);
        x += 10;
    }
    AnimatorSprite_Draw((u8 *)G + 0x5a0);
    AnimatorSprite_Draw((u8 *)G + 0x5e8);
    t = ExtraBoss_GetWork()->unk44;
    off = 0;
    if (t > 0x7e90) {
        *(s32 *)((u8 *)G + 0x79c) += 1;
        if (*(s32 *)((u8 *)G + 0x79c) & 8)
            off = 10;
    }
    if (t > 0x87b3 && t < 0x8c9f && FX_ModS32(t, 0x78) == 0)
        Sound_PlaySeqArc(0, 0, 0x36);
    if (t < 0x8c9f) {
        d[0] = off + FX_DivS32(t, 0xe10);
        t = FX_ModS32(t, 0xe10);
        d[2] = off + FX_DivS32(t, 0x258);
        t = FX_ModS32(t, 0x258);
        d[3] = off + FX_DivS32(t, 0x3c);
        r = FX_ModS32(t, 0x3c) * 100 / 60;
        d[5] = off + FX_DivS32(r, 10);
        r = FX_ModS32(r, 10);
        d[6] = r + off;
    } else {
        u16 v = off + 9;
        d[0] = v;
        d[2] = off + 5;
        d[3] = v;
        d[5] = v;
        d[6] = v;
    }
    d[1] = 0x17;
    d[4] = 0x17;
    AnimatorSprite_Draw((u8 *)G + 0x630);
    for (i = 0; i < 7; i++) {
        e = &G[d[i]];
        e->x = data_ov001_02316e84[i];
        e->y = 0x12;
        AnimatorSprite_Draw(e);
    }
    q = ExtraBoss_GetWork();
    lives = *(s8 *)((u8 *)q + ExtraBoss_GetWork()->unk3C + 0x60);
    if (lives > 9)
        lives = 9;
    else if (lives < 0)
        lives = 0;
    AnimatorSprite_Draw(&G[ExtraBoss_GetWork()->unk3C + 0x19]);
    AnimatorSprite_Draw((u8 *)G + 0x6c0);
    e = &G[lives];
    e->x = 0x26;
    e->y = 0xbc;
    AnimatorSprite_Draw(e);
}
