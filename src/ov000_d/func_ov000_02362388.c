#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 unk8;
    u16 unkA;
} UnkHdr_02362388;

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
} UnkRes_02362388;

typedef struct {
    u8 pad0[0x2C];
    u32 unk2C;
    u8 pad30[0x44 - 0x30];
    u16 unk44;
    u16 unk46;
} UnkSpr_02362388;

s32 func_02039c18(s32 a, u16 b);
void func_0203363c(UnkSpr_02362388 *spr, UnkRes_02362388 *res, s32 idx, s32 a, s32 b, s32 c, s32 pal, s32 d, void *dst);

#pragma thumb on

void func_ov000_02362388(UnkSpr_02362388 *spr, UnkRes_02362388 *res, s32 idx, s32 prio, u16 y) {
    s32 pal = func_02039c18(0, ((UnkHdr_02362388 *)((u8 *)res + res->unk18))->unk8);

    func_0203363c(spr, res, idx, 0, 0, 0, pal, 0, (void *)0x5000200);
    spr->unk2C &= ~0x60000;
    spr->unk2C |= prio << 17;
    spr->unk46 = y;
}

#pragma thumb reset
