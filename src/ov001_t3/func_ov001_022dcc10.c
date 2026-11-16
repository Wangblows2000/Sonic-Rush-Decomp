#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022dcc10;

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x48 - 8];
    s32 unk48;
    u8 pad4c[0xb44 - 0x4c];
    UnkAnim_022dcc10 anims[6];
} UnkWork_022dcc10;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dcc10 *work;
} UnkTask_022dcc10;

extern UnkTask_022dcc10 *data_022b4574;

void func_ov001_022da70c(UnkWork_022dcc10 *w);
void func_ov001_022da154(UnkWork_022dcc10 *w, s32 a);
void func_02040ec0(UnkAnim_022dcc10 *a, s32 id);
void func_ov001_022d6dc0(void);
void func_ov001_022d97a8(s32 a, s32 b, s32 c, s32 d);
void func_02051db0(s32 a, s32 b, s32 c, s32 d);
void func_ov001_022dc998(void);

#pragma thumb on
void func_ov001_022dcc10(void) {
    UnkWork_022dcc10 *w;

    w = data_022b4574->work;
    func_ov001_022da70c(w);
    w->timer++;
    if (w->timer >= 0x3c) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da154(w, 1);
        w->unk48 = 0;
        func_02040ec0(&w->anims[0], 0xe);
        func_02040ec0(&w->anims[1], 0xf);
        func_02040ec0(&w->anims[2], 0x10);
        func_02040ec0(&w->anims[3], 0x11);
        func_02040ec0(&w->anims[4], 0x12);
        func_02040ec0(&w->anims[5], 0x13);
        func_ov001_022d6dc0();
        func_ov001_022d97a8(0, 0x10, 0x10, 0xc);
        func_02051db0(2, 0xc, 0x3000, 0);
        data_022b4574->func = func_ov001_022dc998;
    }
}
#pragma thumb reset
