#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} UnkSub_022cc588;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    u8 pad1c[0x1ec - 0x1c];
    UnkSub_022cc588 sub;
} UnkWork_022cc588;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022cc588 *work;
} UnkTask_022cc588;

extern UnkTask_022cc588 *gTaskCurrent;

void func_ov001_022cc17c(UnkWork_022cc588 *w);
void func_ov001_022cbf30(UnkWork_022cc588 *w);
s32 func_ov001_022c8d54(UnkSub_022cc588 *s);
void func_ov001_022c8d4c(UnkSub_022cc588 *s);
void func_0205004c(void);
BOOL func_02050018(void);
BOOL func_0205000c(void);
void func_ov001_022cc4a0(void);
void func_ov001_022cc6b8(void);

#pragma thumb on
void func_ov001_022cc588(void) {
    UnkWork_022cc588 *w = gTaskCurrent->work;
    UnkSub_022cc588 *s = &w->sub;

    func_ov001_022cc17c(w);
    func_ov001_022cbf30(w);
    w->sub.unk0++;
    w->unk14++;
    w->unk18++;
    s->unk8 = func_ov001_022c8d54(s);
    if (s->unk8 == 0) {
        func_0205004c();
    }
    if (func_02050018()) {
        if (!func_0205000c() && s->unk8 > 0) {
            s->unk0 = 0;
            w->unk14 = 0;
            w->unk18 = 0;
            s->unk4 = 8;
            gTaskCurrent->fn = func_ov001_022cc4a0;
            func_ov001_022c8d4c(s);
            return;
        }
        s->unk4 = 5;
        gTaskCurrent->fn = func_ov001_022cc6b8;
    }
}
#pragma thumb reset
