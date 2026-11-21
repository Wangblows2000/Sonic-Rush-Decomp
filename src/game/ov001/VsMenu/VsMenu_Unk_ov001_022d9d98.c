#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x3e];
    u16 unk46;
} UnkSpr_022d9d98;

typedef struct {
    u8 pad0[0x1c];
    u8 sub[0xc];
} UnkItem_022d9d98;

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    u8 pad1c[8];
    UnkItem_022d9d98 items[8];
    u8 pad[0x210 - 0x24 - 8 * 0x28];
    UnkSpr_022d9d98 spr[8];
} UnkStruct_022d9d98;

void AnimatorSprite_ProcessAnimation(UnkSpr_022d9d98 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022d9d98 *s);
void VsMenu_Unk_ov001_022d9864(UnkStruct_022d9d98 *p, UnkItem_022d9d98 *a, void *b, s32 x, s32 y, s32 f);

#pragma thumb on
void VsMenu_Unk_ov001_022d9d98(UnkStruct_022d9d98 *p, s32 i, s32 x, s32 y) {
    UnkSpr_022d9d98 *s = &p->spr[i];
    UnkItem_022d9d98 *a;
    void *b;
    s32 tx;
    s32 ty;

    s->x = x;
    s->y = y;
    s->unk46 = 4;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    a = &p->items[i];
    b = p->items[i].sub;
    tx = x + 0x30;
    ty = y + 7;
    switch (i) {
    case 0:
        tx += 0x40;
        ty += 0x2c;
        break;
    case 1:
        tx += 0x40;
        ty += 0x5c;
        break;
    }
    if (p->unk18 != 0) {
        VsMenu_Unk_ov001_022d9864(p, a, b, tx, ty, 1);
    } else {
        VsMenu_Unk_ov001_022d9864(p, a, b, tx, ty, 0);
    }
}
#pragma thumb reset
