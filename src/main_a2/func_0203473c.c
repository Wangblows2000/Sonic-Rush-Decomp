#include <nitro/types.h>
#include "main_a2/bgdual.h"

extern u32 data_02072c80[];

void func_02039200(u32 idx, u32 e, u32 *a, u32 *b);
void func_020391bc(u32 idx, u32 e, u16 *x, u16 *y);
void func_02038a4c(u32 idx, u32 e, u32 *a, u16 *b, u16 *c);

void func_0203473c(UnkBgDual *b, UnkBgRes *res, u32 a, u32 idx, u32 e, u16 f, u16 g) {
    u16 x, y;
    UNK_BG_SETUP_COMMON(b, res, a);
    b->unk30 = (u8 *)res + res->ofs10;
    b->unk34 = (u8 *)res + res->ofs8;
    b->unk38 = (u8 *)res + res->ofsC;
    b->unk3c = res->unk18;
    b->unk3e = res->unk1a;
    b->idx = idx;
    b->unk10 = e;
    func_02039200(idx, e, &b->unk14, &b->unk18);
    b->unk18 = b->unk18 + res->unk1c * 2 + res->unk1e * 512;
    func_020391bc(idx, e, &x, &y);
    b->unk1c = data_02072c80[idx] + ((x << 16) + (y << 14));
    func_02038a4c(idx, e, &b->unk20, &b->unk24, &b->unk26);
    b->unk28 = 0;
    b->unk2a = 0;
    b->unk2c = f;
    b->unk2e = g;
}
