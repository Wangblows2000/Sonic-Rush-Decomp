#include <nitro/types.h>
#include "game/Graphics/bgdual.h"

void Background_Draw(UnkBgDual *b);

void BackgroundDS_Draw(UnkBgDual *b) {
    b->idx = 0;
    b->unk10 = b->unk50[0];
    b->unk4 = b->unk40[0].a;
    b->unk8 = b->unk40[0].b;
    b->unk14 = b->unk54[0];
    b->unk18 = b->unk5c[0];
    b->unk1c = b->unk64[0];
    b->unk20 = b->unk6c[0];
    b->unk24 = b->unk74[0];
    b->unk26 = b->unk78[0];
    b->unk28 = b->unk7c[0].a;
    b->unk2a = b->unk7c[0].b;
    Background_Draw(b);
    b->idx = 1;
    b->unk10 = b->unk50[1];
    b->unk4 = b->unk40[1].a;
    b->unk8 = b->unk40[1].b;
    b->unk14 = b->unk54[1];
    b->unk18 = b->unk5c[1];
    b->unk1c = b->unk64[1];
    b->unk20 = b->unk6c[1];
    b->unk24 = b->unk74[1];
    b->unk26 = b->unk78[1];
    b->unk28 = b->unk7c[1].a;
    b->unk2a = b->unk7c[1].b;
    Background_Draw(b);
}
