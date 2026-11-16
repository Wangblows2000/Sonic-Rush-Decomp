#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 data_ov000_0236aad8;

s16 func_02068db4(s32 v, s32 d);
void func_ov000_023218d0(UnkStruct_EffObj *w);

void func_ov000_023216cc(UnkStruct_EffObj *w) {
    s32 v;

    if (w->unkC != 0)
        w->unkC--;
    else
        w->unk2E = func_02068db4(w->unk2E, 8);
    v = w->unk2E;
    if (v < 0)
        v = -v;
    if (v < 0x40) {
        func_ov000_023218d0(w);
        return;
    }
    v = (data_ov000_0236aad8.unk62 + 8) << 8;
    if (w->unk14 < v)
        w->unk14 = v;
}
