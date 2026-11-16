#include <nitro/types.h>

typedef struct {
    u32 timer;
    s32 state;
} UnkSub_022cc33c;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    u8 pad1c[0x1ec - 0x1c];
    UnkSub_022cc33c sub;
} UnkWork_022cc33c;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cc33c *work;
} UnkTask_022cc33c;

extern UnkTask_022cc33c *gTaskCurrent;

void func_ov001_022cc17c(UnkWork_022cc33c *w);
void func_ov001_022cbf30(UnkWork_022cc33c *w);
BOOL func_02050018(void);
BOOL func_0205000c(void);
void func_ov001_022c8cc0(UnkSub_022cc33c *s);
void func_ov001_022cdd28(void);
void func_ov001_022ccb5c(void);
void func_ov001_022cca00(void);
void func_ov001_022cc998(void);
void func_ov001_022cc8ec(void);
void func_ov001_022cc7fc(void);
void func_ov001_022cc6b8(void);
void func_ov001_022cc588(void);
void func_ov001_022cc4a0(void);
void func_ov001_022cc268(void);
void func_ov001_022cc210(void);

#pragma thumb on
void func_ov001_022cc33c(void) {
    UnkWork_022cc33c *w;
    UnkSub_022cc33c *sub;

    w = gTaskCurrent->work;
    sub = &w->sub;
    func_ov001_022cc17c(w);
    func_ov001_022cbf30(w);
    w->sub.timer++;
    w->unk14++;
    w->unk18++;
    if (!func_02050018()) return;
    if (!func_0205000c()) {
        sub->timer = 0;
        w->unk14 = 0;
        w->unk18 = 0;
        func_ov001_022c8cc0(sub);
        func_ov001_022cdd28();
        return;
    }
    switch (sub->state) {
    case 0:
        gTaskCurrent->func = func_ov001_022ccb5c;
        return;
    case 1:
        gTaskCurrent->func = func_ov001_022cca00;
        return;
    case 2:
        gTaskCurrent->func = func_ov001_022cc998;
        return;
    case 3:
        gTaskCurrent->func = func_ov001_022cc8ec;
        return;
    case 4:
        gTaskCurrent->func = func_ov001_022cc7fc;
        return;
    case 5:
        gTaskCurrent->func = func_ov001_022cc6b8;
        return;
    case 7:
        gTaskCurrent->func = func_ov001_022cc588;
        return;
    case 8:
        gTaskCurrent->func = func_ov001_022cc4a0;
        return;
    case 9:
        gTaskCurrent->func = func_ov001_022cc268;
        return;
    case 10:
        gTaskCurrent->func = func_ov001_022cc210;
        return;
    case 6:
    default:
        func_ov001_022c8cc0(sub);
        func_ov001_022cdd28();
        break;
    }
}
#pragma thumb reset
