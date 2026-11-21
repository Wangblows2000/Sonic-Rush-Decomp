#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
} UnkStruct_0236aad8;

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x18c - 0x134];
    s32 unk18C;
} UnkWork_0232179c;

extern UnkStruct_0236aad8 gCamera;

s16 ObjSpd_DownSet(s32 v, s32 d);
void DecGroup41_ChooseNextAction(UnkWork_0232179c *w);

void DecGroup41_State_Sink(UnkWork_0232179c *w) {
    s32 v;

    w->base.unk2E = ObjSpd_DownSet(w->base.unk2E, 8);
    if (w->base.unkC == 0) {
        DecGroup41_ChooseNextAction(w);
        return;
    }
    w->base.unkC--;
    if (w->base.unk14 > w->unk18C * 0x100 + 0x4000) {
        DecGroup41_ChooseNextAction(w);
        return;
    }
    v = (gCamera.unk62 + 8) << 8;
    if (w->base.unk14 < v)
        w->base.unk14 = v;
}
