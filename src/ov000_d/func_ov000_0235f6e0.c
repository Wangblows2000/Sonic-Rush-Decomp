#include <nitro/types.h>

typedef struct {
    void *unk0;
    u32 unk4;
} UnkStruct_0235f6e0;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_0235f6e0 *work;
} UnkStruct_0235f6e0_Task;

extern UnkStruct_0235f6e0_Task *data_022b4574;

s32 func_020428d4(void);
void func_02033550(void *spr, u16 anim);
void func_0203344c(void *spr, s32 a, s32 b);
void func_02032d2c(void *spr);

#pragma thumb on

void func_ov000_0235f6e0(void) {
    UnkStruct_0235f6e0 *w = data_022b4574->work;
    u32 anim;

    switch (func_020428d4()) {
    case 0:
        anim = 1;
        break;
    case 1:
        anim = 2;
        break;
    case 2:
        anim = 3;
        break;
    case 3:
    default:
        anim = 4;
        break;
    }
    if (w->unk4 != anim) {
        w->unk4 = anim;
        func_02033550(w->unk0, w->unk4);
    }
    func_0203344c(w->unk0, 0, 0);
    func_02032d2c(w->unk0);
}

#pragma thumb reset
