#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
} UnkStruct_0236aad8;

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x190 - 0x134];
    s32 unk190;
} UnkWork_02321188;

extern UnkStruct_0236aad8 gCamera;

s16 ObjSpd_DownSet(s32 v, s32 d);
void DecGroup41_ChooseNextAction(UnkWork_02321188 *w);

void DecGroup41_State_Flee(UnkWork_02321188 *w) {
    s32 v;

    if (w->base.unkC != 0) {
        w->base.unkC--;
    } else {
        w->base.unk2E = ObjSpd_DownSet(w->base.unk2E, 0x20);
        w->base.unk30 = ObjSpd_DownSet(w->base.unk30, 0x20);
        w->unk190 = 0;
    }
    v = (gCamera.unk62 + 8) << 8;
    if (w->base.unk14 < v)
        w->base.unk14 = v;
    v = w->base.unk2E;
    if (v < 0)
        v = -v;
    if (v >= 0x40)
        return;
    v = w->base.unk30;
    if (v < 0)
        v = -v;
    if (v >= 0x40)
        return;
    w->base.unk58 |= 0x80;
    w->base.unk54 &= ~2;
    DecGroup41_ChooseNextAction(w);
}
