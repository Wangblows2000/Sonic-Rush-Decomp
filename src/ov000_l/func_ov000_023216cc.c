#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 gCamera;

s16 ObjSpd_DownSet(s32 v, s32 d);
void func_ov000_023218d0(UnkStruct_EffObj *w);

void func_ov000_023216cc(UnkStruct_EffObj *w) {
    s32 v;

    if (w->unkC != 0)
        w->unkC--;
    else
        w->unk2E = ObjSpd_DownSet(w->unk2E, 8);
    v = w->unk2E;
    if (v < 0)
        v = -v;
    if (v < 0x40) {
        func_ov000_023218d0(w);
        return;
    }
    v = (gCamera.unk62 + 8) << 8;
    if (w->unk14 < v)
        w->unk14 = v;
}
