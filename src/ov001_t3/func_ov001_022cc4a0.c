#include <nitro/types.h>

typedef struct {
    u32 timer;
    s32 unk4;
    u8 pad8[8];
    s32 unk10;
    s32 unk14;
} UnkSub_022cc4a0;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x1ec - 0x20];
    UnkSub_022cc4a0 sub;
} UnkWork_022cc4a0;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cc4a0 *work;
} UnkTask_022cc4a0;

extern UnkTask_022cc4a0 *gTaskCurrent;

void func_ov001_022cc17c(UnkWork_022cc4a0 *w);
void func_ov001_022cbf30(UnkWork_022cc4a0 *w);
s32 func_ov001_022c8d44(UnkSub_022cc4a0 *s);
s32 func_ov001_022c8d0c(UnkSub_022cc4a0 *s);
BOOL func_ov001_022c8cd4(UnkSub_022cc4a0 *s);
void func_ov001_022c8d14(UnkSub_022cc4a0 *s);
void func_ov001_022c8cc0(UnkSub_022cc4a0 *s);
void SysSound_PlaySE(s32 a);
void func_ov001_022cc268(void);
void func_ov001_022cca00(void);

#pragma thumb on
void func_ov001_022cc4a0(void) {
    UnkWork_022cc4a0 *w;
    UnkSub_022cc4a0 *sub;
    BOOL done;
    BOOL ok;

    w = gTaskCurrent->work;
    sub = &w->sub;
    func_ov001_022cc17c(w);
    func_ov001_022cbf30(w);
    w->sub.timer++;
    w->unk14++;
    w->unk18++;
    done = FALSE;
    ok = FALSE;
    if (sub->unk10 == 0) {
        sub->unk10 = func_ov001_022c8d44(sub);
        if (sub->unk10 != 0) {
            if (func_ov001_022c8cd4(sub)) {
                ok = TRUE;
            } else {
                func_ov001_022c8d14(sub);
            }
        }
    } else if (sub->unk14 == 0) {
        sub->unk14 = func_ov001_022c8d0c(sub);
        if (sub->unk14 != 0) {
            if (func_ov001_022c8cd4(sub)) {
                ok = TRUE;
            } else {
                done = TRUE;
            }
        }
    } else {
        done = TRUE;
    }
    if (done) {
        sub->timer = 0;
        w->unk14 = 0;
        w->unk18 = 0;
        sub->unk4 = 9;
        func_ov001_022c8cc0(sub);
        gTaskCurrent->func = func_ov001_022cc268;
        return;
    }
    if (ok) {
        SysSound_PlaySE(6);
        sub->timer = 0;
        w->unk14 = 0;
        w->unk18 = 0;
        sub->unk4 = 1;
        func_ov001_022c8cc0(sub);
        gTaskCurrent->func = func_ov001_022cca00;
    }
}
#pragma thumb reset
