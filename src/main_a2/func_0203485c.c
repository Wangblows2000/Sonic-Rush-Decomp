#include <nitro/types.h>
#include "main_a2/bgdual.h"

void func_0203485c(UnkBgDual *b, UnkBgRes *res, u32 a, u32 idx, u8 e, u32 v14, u32 base, u32 v1c, u32 v20, u16 v24, u16 v26, u16 v28, u16 v2a, u16 v2c, u16 v2e) {
    UNK_BG_SETUP_COMMON(b, res, a);
    b->unk10 = e;
    b->unk30 = (u8 *)res + res->ofs10;
    b->unk34 = (u8 *)res + res->ofs8;
    b->unk38 = (u8 *)res + res->ofsC;
    b->unk3c = res->unk18;
    b->unk3e = res->unk1a;
    b->idx = idx;
    b->unk14 = v14;
    b->unk18 = base + res->unk1c * 2;
    b->unk1c = v1c;
    b->unk20 = v20;
    b->unk24 = v24;
    b->unk26 = v26;
    b->unk28 = v28;
    b->unk2a = v2a;
    b->unk2c = v2c;
    b->unk2e = v2e;
}
