#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x1c4 - 0x134];
    u8 spr[8];
    u16 unk1CC;
} UnkWork_02327398;

void EffKemuriSpin3D_State_Main(UnkWork_02327398 *w) {
    if (!(w->base.unk5C & 8))
        return;
    if (w->unk1CC == 1 || w->unk1CC == 3) {
        w->base.unk54 |= 4;
        return;
    }
    AnimatorSprite_SetAnimation((UnkStruct_Spr *)w->spr, w->unk1CC);
    w->base.unk5C &= ~8;
}
