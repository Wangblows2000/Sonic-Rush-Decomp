#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x238 - 0x20C];
    u32 unk238;
} UnkStruct_02301108_P;

void KuragenBomb_Action_FollowCarrier(UnkStruct_StageObj *w);

void KuragenBomb_State_Hold(UnkStruct_StageObj *w) {
    UnkStruct_02301108_P *p = w->unk88;

    w->unkC--;
    if (w->unkC == 0) {
        if (p != NULL)
            p->unk238 |= 0x100;
        KuragenBomb_Action_FollowCarrier(w);
        return;
    }
    if (p == NULL)
        return;
    w->unk10 = p->base.unk10;
    w->unk14 = p->base.unk14 + (((0x3c - w->unkC) * 0xf) << 8) / 60;
}
