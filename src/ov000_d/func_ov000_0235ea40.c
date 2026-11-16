#include <ov000_d/sprobj.h>

typedef struct {
    u8 pad0[8];
    s16 unk8;
} UnkStruct_0235ea40_Bright;

extern UnkStruct_0235ea40_Bright data_0208926c;
extern UnkStruct_0235ea40_Bright data_02087d44;

s32 func_ov000_0235c9f8(s32 from, s32 to, s32 total, s32 cur, s32 bias);
void func_ov000_0235e94c(void);

#pragma thumb on

void func_ov000_0235ea40(void) {
    UnkStruct_SprObj *w = gTaskCurrent->work;

    func_ov000_0235e124(w);
    func_ov000_0235e068(w);
    func_ov000_0235e018(w);
    func_ov000_0235dfc4(w);
    if ((s32)w->unk0 >= 0) {
        s16 v = func_ov000_0235c9f8(w->unk14, 0, 0x10, w->unk0, 0x1000);
        data_0208926c.unk8 = v;
        data_02087d44.unk8 = v;
    }
    w->unk0++;
    if (w->unk0 >= 0x20) {
        w->unk0 = 0;
        gTaskCurrent->unk8 = func_ov000_0235e94c;
    }
}

#pragma thumb reset
