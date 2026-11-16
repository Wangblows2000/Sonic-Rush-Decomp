#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022da91c;

typedef struct {
    u8 pad0[0xb44];
    UnkAnim_022da91c anims[14];
} UnkWork_022da91c;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
} UnkTask_022da91c;

extern UnkTask_022da91c *gTaskCurrent;

void func_ov001_022da374(UnkWork_022da91c *w);
void func_ov001_022da33c(UnkWork_022da91c *w);
void func_ov001_022d9f6c(UnkWork_022da91c *w);
void func_ov001_022d9f2c(UnkWork_022da91c *w);
void func_02040ec0(UnkAnim_022da91c *a, s32 idx);
void func_02040e80(UnkAnim_022da91c *a);
void func_ov001_022d6dac(void);
void func_ov001_022db5f4(void);

#pragma thumb on
void func_ov001_022da91c(UnkWork_022da91c *w) {
    s32 i;

    func_ov001_022da374(w);
    func_ov001_022da33c(w);
    func_ov001_022d9f6c(w);
    func_ov001_022d9f2c(w);
    func_02040ec0(&w->anims[0], 0);
    func_02040ec0(&w->anims[1], 1);
    func_02040e80(&w->anims[0]);
    func_02040e80(&w->anims[1]);
    for (i = 0; i < 8; i++) {
        UnkAnim_022da91c *a = &w->anims[i + 6];
        func_02040ec0(a, i + 6);
        func_02040e80(a);
    }
    func_ov001_022d6dac();
    gTaskCurrent->fn = func_ov001_022db5f4;
}
#pragma thumb reset
