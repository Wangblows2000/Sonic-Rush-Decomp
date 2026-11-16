#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkTask_02062c4c;

extern void *data_022c4da8;

void func_02061870(void *f);
BOOL func_02061a78(void *o);
void func_02061fc4(void);
void func_020612c0(void *w);

#define REG_DISPCNT_ (*(vu32 *)0x04000000)

#pragma thumb on
void func_02062c4c(UnkTask_02062c4c *t) {
    void *w = t->work;

    func_02061870(NULL);
    func_02061a78(NULL);
    {
        u32 plane = (REG_DISPCNT_ & 0x1f00) >> 8;
        REG_DISPCNT_ = (REG_DISPCNT_ & ~0x1f00) | ((plane | 4) << 8);
    }
    func_02061fc4();
    func_020612c0(w);
    data_022c4da8 = NULL;
}
#pragma thumb reset
