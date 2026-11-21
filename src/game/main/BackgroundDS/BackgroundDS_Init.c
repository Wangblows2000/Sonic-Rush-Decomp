#include <nitro/types.h>
#include "main_a2/bgdual.h"

void Background_Init(UnkBgDual *b, u32 a, u32 c, u32 idx, u8 e, u16 f, u16 g);

void BackgroundDS_Init(UnkBgDual *b, u32 a, u32 c, u8 e0, u8 e1, u16 f, u16 g) {
    Background_Init(b, a, c, 0, e0, f, g);
    b->unk50[0] = e0;
    b->unk54[0] = b->unk14;
    b->unk5c[0] = b->unk18;
    b->unk64[0] = b->unk1c;
    b->unk6c[0] = b->unk20;
    b->unk74[0] = b->unk24;
    b->unk78[0] = b->unk26;
    b->unk7c[0].a = b->unk28;
    b->unk7c[0].b = b->unk2a;
    Background_Init(b, a, c, 1, e1, f, g);
    b->unk50[1] = e1;
    b->unk54[1] = b->unk14;
    b->unk5c[1] = b->unk18;
    b->unk64[1] = b->unk1c;
    b->unk6c[1] = b->unk20;
    b->unk74[1] = b->unk24;
    b->unk78[1] = b->unk26;
    b->unk7c[1].a = b->unk28;
    b->unk7c[1].b = b->unk2a;
}
