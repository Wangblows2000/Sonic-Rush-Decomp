#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
} UnkStruct_0236aad8;

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x18c - 0x134];
    s32 unk18C;
} UnkWork_0232179c;

extern UnkStruct_0236aad8 data_ov000_0236aad8;

s16 ObjSpd_DownSet(s32 v, s32 d);
void func_ov000_023218d0(UnkWork_0232179c *w);

void func_ov000_0232179c(UnkWork_0232179c *w) {
    s32 v;

    w->base.unk2E = ObjSpd_DownSet(w->base.unk2E, 8);
    if (w->base.unkC == 0) {
        func_ov000_023218d0(w);
        return;
    }
    w->base.unkC--;
    if (w->base.unk14 > w->unk18C * 0x100 + 0x4000) {
        func_ov000_023218d0(w);
        return;
    }
    v = (data_ov000_0236aad8.unk62 + 8) << 8;
    if (w->base.unk14 < v)
        w->base.unk14 = v;
}
