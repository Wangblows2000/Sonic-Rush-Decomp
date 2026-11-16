#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 gCamera;

void DecGroup41_State_Unk_ov000_02321378(UnkStruct_EffObj *w) {
    s32 v;

    w->unkC++;
    v = (gCamera.unk62 + 8) << 8;
    if (w->unk14 < v)
        w->unk14 = v;
    if (w->unkC & 0x1f)
        return;
    if (Rand() & 3)
        return;
    w->unk30 = 0xff - (Rand() & 0x1fe);
}
