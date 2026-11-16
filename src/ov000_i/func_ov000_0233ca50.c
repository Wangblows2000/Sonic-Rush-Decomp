#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x20 - 0xc];
    s16 unk20;
    u8 pad22[0x62 - 0x22];
    u16 unk62;
} UnkPlayer_0233ca50;

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkSpr_0233ca50;

extern UnkPlayer_0233ca50 gCamera[2];

s32 func_0200225c(s32 a, s32 b);
void AnimatorSprite_ProcessAnimation(void *p, s32 a, s32 b);
void AnimatorSprite_Draw(void *p);

void func_ov000_0233ca50(u8 *w) {
    u8 i;
    s16 x;
    s16 y;
    s32 v;
    UnkSpr_0233ca50 *sp;
    UnkPlayer_0233ca50 *p;
    UnkSpr_0233ca50 *s;

    if (*(u32 *)((u8 *)gCamera + 0xc8) & 0x2000000)
        return;
    s = (UnkSpr_0233ca50 *)(w + 0x8bc);
    AnimatorSprite_ProcessAnimation(&s[0], 0, 0);
    AnimatorSprite_ProcessAnimation(&s[1], 0, 0);
    for (i = 0; i < 2; i++) {
        p = &gCamera[i];
        if ((p->flags & 0x1000000) && !(p->flags & 0x2000000)) {
            v = func_0200225c((p->unk62 << 8) - p->unk8, p->unk20);
            y = (s16)((u32)(v >> 4) >> 8);
            if (y >= -3 && y <= 0xc3) {
                sp = &((UnkSpr_0233ca50 *)(w + 0x8bc))[i];
                sp->y = y;
                for (x = -0x20; x < 0x100; x += 0x40) {
                    sp->x = x;
                    AnimatorSprite_Draw(sp);
                }
            }
        }
    }
}
