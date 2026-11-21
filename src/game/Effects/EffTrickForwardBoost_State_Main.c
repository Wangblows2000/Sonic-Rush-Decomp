#include <game/Stage/stageobj_alt.h>

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x328 - 0x134];
    s16 unk328;
} UnkPlayer_023312f0;

void EffTrickForwardBoost_State_Main(UnkStruct_EffObj *w) {
    UnkPlayer_023312f0 *p = w->unk88;

    if (p == NULL)
        return;
    w->unk10 = p->base.unk10;
    w->unk14 = ((UnkPlayer_023312f0 *)w->unk88)->base.unk14;
    if (((UnkPlayer_023312f0 *)w->unk88)->base.unk5C & 1)
        w->unk5C |= 1;
    else
        w->unk5C &= ~1;
    if (w->unk5C & 1)
        w->unk10 -= w->unk2E * 0x100;
    else
        w->unk10 += w->unk2E * 0x100;
    w->unk14 += w->unk30 * 0x100;
    if (p->unk328 == 0x37 && !(p->base.unk58 & 0x2000))
        return;
    w->unk54 |= 4;
}
