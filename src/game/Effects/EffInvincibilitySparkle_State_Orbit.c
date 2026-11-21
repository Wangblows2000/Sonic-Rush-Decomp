#include <game/Stage/stageobj_alt.h>

extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void EffInvincibilitySparkle_State_Orbit(UnkStruct_EffObj *w) {
    u16 a = (u16)w->unkC + 0x4000;

    w->unk2E = (COS(a) << 9) >> 12;
    w->unk30 = (SIN(a) << 9) >> 12;
    w->unkC += 0x400;
    if (w->unk5C & 8)
        w->unk54 |= 4;
}
