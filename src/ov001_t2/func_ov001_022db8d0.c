#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022db8d0;

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0xb44 - 8];
    UnkAnim_022db8d0 anims[14];
} UnkWork_022db8d0;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022db8d0 *work;
} UnkTask_022db8d0;

extern UnkTask_022db8d0 *data_022b4574;

void func_ov001_022da614(UnkWork_022db8d0 *w);
BOOL func_02050018(void);
void func_ov001_022da374(UnkWork_022db8d0 *w);
void func_ov001_022da33c(UnkWork_022db8d0 *w);
void func_ov001_022d9f6c(UnkWork_022db8d0 *w);
void func_ov001_022d9f2c(UnkWork_022db8d0 *w);
void func_02040ec0(UnkAnim_022db8d0 *a, s32 idx);
void func_02040e80(UnkAnim_022db8d0 *a);
void func_ov001_022d6dac(void);
void func_ov001_022db6ec(void);

#pragma thumb on
void func_ov001_022db8d0(void) {
    UnkWork_022db8d0 *w = data_022b4574->work;
    s32 i;

    func_ov001_022da614(w);
    w->timer++;
    if (!func_02050018()) {
        return;
    }
    w->timer = 0;
    w->unk4 = 0;
    func_ov001_022da374(w);
    func_ov001_022da33c(w);
    func_ov001_022d9f6c(w);
    func_ov001_022d9f2c(w);
    func_02040ec0(&w->anims[0], 0);
    func_02040ec0(&w->anims[1], 1);
    func_02040e80(&w->anims[0]);
    func_02040e80(&w->anims[1]);
    for (i = 0; i < 8; i++) {
        UnkAnim_022db8d0 *a = &w->anims[i + 6];
        func_02040ec0(a, i + 6);
        func_02040e80(a);
    }
    func_ov001_022d6dac();
    data_022b4574->fn = func_ov001_022db6ec;
}
#pragma thumb reset
