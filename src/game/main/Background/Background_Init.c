#include <nitro/types.h>
#include "main_a2/bgdual.h"

extern u32 data_02072c80[];

void Bg_GetPaletteDest(u32 idx, u32 e, u32 *a, u32 *b);
void Bg_GetCharBase(u32 idx, u32 e, u16 *x, u16 *y);
void Bg_GetScreenFormat(u32 idx, u32 e, u32 *a, u16 *b, u16 *c);

void Background_Init(UnkBgDual *b, UnkBgRes *res, u32 a, u32 idx, u32 e, u16 f, u16 g) {
    u16 x, y;
    UNK_BG_SETUP_COMMON(b, res, a);
    b->unk30 = (u8 *)res + res->ofs10;
    b->unk34 = (u8 *)res + res->ofs8;
    b->unk38 = (u8 *)res + res->ofsC;
    b->unk3c = res->unk18;
    b->unk3e = res->unk1a;
    b->idx = idx;
    b->unk10 = e;
    Bg_GetPaletteDest(idx, e, &b->unk14, &b->unk18);
    b->unk18 = b->unk18 + res->unk1c * 2 + res->unk1e * 512;
    Bg_GetCharBase(idx, e, &x, &y);
    b->unk1c = data_02072c80[idx] + ((x << 16) + (y << 14));
    Bg_GetScreenFormat(idx, e, &b->unk20, &b->unk24, &b->unk26);
    b->unk28 = 0;
    b->unk2a = 0;
    b->unk2c = f;
    b->unk2e = g;
}
