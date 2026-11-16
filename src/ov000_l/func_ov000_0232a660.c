#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xe0];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 gCamera;

void func_ov000_022c5e80(s32 a, s32 b, s32 x, s32 *ox, s32 *oy, s32 *oz);

void func_ov000_0232a660(UnkStruct_EffObj *w) {
    s32 z;
    s32 x;
    s32 y;

    if (w->unk88 != NULL) {
        w->unk10 = ((UnkStruct_EffObj *)w->unk88)->unk10 + w->unk2E;
        w->unk14 = ((UnkStruct_EffObj *)w->unk88)->unk14 + w->unk30;
        func_ov000_022c5e80(gCamera.unkE0, gCamera.unkE4, w->unk10 << 4, &x, &y, &z);
        w->unk10 = x >> 4;
        w->unk18 = y >> 4;
    }
    w->unkC++;
    if (w->unkC > 0x20)
        w->unk54 |= 4;
}
