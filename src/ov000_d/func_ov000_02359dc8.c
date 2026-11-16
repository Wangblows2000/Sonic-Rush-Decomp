#include <nitro/types.h>

typedef struct {
    u8 pad0[0xE];
    u16 unkE;
    u16 unk10;
    u8 pad12[2];
} UnkEntry_02359dc8;

typedef struct {
    u8 pad0[0x10];
    UnkEntry_02359dc8 entries[1];
} UnkHdr_02359dc8;

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
} UnkRes_02359dc8;

typedef struct {
    u8 pad0[0x2C];
    u32 unk2C;
    u8 pad30[0x44 - 0x30];
    u16 unk44;
    u16 unk46;
} UnkSpr_02359dc8;

s32 func_02039c18(s32 a, u16 b);
void func_0203363c(UnkSpr_02359dc8 *spr, UnkRes_02359dc8 *res, s32 idx, s32 a, s32 b, s32 c, s32 pal, s32 d, void *dst);

static inline u16 UnkGet_02359dc8(UnkRes_02359dc8 *res, s32 idx) {
    UnkEntry_02359dc8 *e = ((UnkHdr_02359dc8 *)((u8 *)res + res->unk18))->entries;
    return e[idx].unkE;
}

#pragma thumb on

void func_ov000_02359dc8(UnkSpr_02359dc8 *spr, UnkRes_02359dc8 *res, s32 idx, s32 prio, u16 x, u16 y) {
    s32 pal = func_02039c18(0, UnkGet_02359dc8(res, idx));

    func_0203363c(spr, res, idx, 0, 0, 0, pal, 0, (void *)0x5000200);
    spr->unk2C &= ~0x60000;
    spr->unk2C |= prio << 17;
    spr->unk44 = x;
    spr->unk46 = y;
}

#pragma thumb reset
