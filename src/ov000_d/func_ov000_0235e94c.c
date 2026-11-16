#include <ov000_d/sprobj.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_0235e94c_Sync;

UnkStruct_0235e94c_Sync *func_020514e4(void);
s32 func_ov000_0235c9f8(s32 from, s32 to, s32 total, s32 cur, s32 bias);
void func_ov000_0235e8ac(void);

#pragma thumb on

void func_ov000_0235e94c(void) {
    UnkStruct_SprObj *w = gTaskCurrent->work;
    s32 t;

    func_ov000_0235e124(w);
    func_ov000_0235e068(w);
    func_ov000_0235e018(w);
    if (w->unk0 == 0) {
        switch (w->unk20) {
        case 0:
            if (w->unk18 == 0) {
                func_ov000_0235d888(w, 1);
                func_ov000_0235d7b4(w, 2);
            } else {
                func_ov000_0235d888(w, 2);
                func_ov000_0235d7b4(w, 1);
            }
            break;
        case 1:
            if (w->unk18 == 0) {
                func_ov000_0235d888(w, 2);
                func_ov000_0235d7b4(w, 1);
            } else {
                func_ov000_0235d888(w, 1);
                func_ov000_0235d7b4(w, 2);
            }
            break;
        case 2:
            func_ov000_0235d888(w, 2);
            func_ov000_0235d7b4(w, 2);
            break;
        }
    }
    func_ov000_0235dfc4(w);
    t = w->unk0 - 0x38;
    if (t >= 0) {
        func_ov000_0235def0(w, w->unk20, func_ov000_0235c9f8(0, 0x1000, 8, t, 0x1000));
    }
    w->unk0++;
    if (w->unk0 >= 0x40) {
        UnkStruct_0235e94c_Sync *s = func_020514e4();
        w->unk0 = 0;
        s->unk3 = 0;
        func_ov000_0235cfc4(w);
        gTaskCurrent->unk8 = func_ov000_0235e8ac;
    }
}

#pragma thumb reset
