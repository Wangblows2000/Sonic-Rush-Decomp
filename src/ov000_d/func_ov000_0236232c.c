#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 unk8;
    u16 unkA;
} UnkHdr_0236232c;

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
} UnkRes_0236232c;

typedef struct {
    u8 pad0[0x2C];
    u32 unk2C;
    u8 pad30[0x44 - 0x30];
    u16 unk44;
    u16 unk46;
} UnkSpr_0236232c;

s32 ObjVram_Alloc(s32 a, u16 b);
void AnimatorSprite_Init(UnkSpr_0236232c *spr, UnkRes_0236232c *res, s32 idx, s32 a, s32 b, s32 c, s32 pal, s32 d, void *dst);

#pragma thumb on

void func_ov000_0236232c(UnkSpr_0236232c *spr, UnkRes_0236232c *res, s32 idx, s32 prio, u16 y) {
    s32 pal = ObjVram_Alloc(1, ((UnkHdr_0236232c *)((u8 *)res + res->unk18))->unkA);

    AnimatorSprite_Init(spr, res, idx, 0, 1, 0, pal, 0, (void *)0x5000600);
    spr->unk2C &= ~0x60000;
    spr->unk2C |= prio << 17;
    spr->unk46 = y;
}

#pragma thumb reset
