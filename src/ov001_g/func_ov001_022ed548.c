#include <nitro/types.h>

typedef struct UnkWork_022ed548 {
    s32 timer;
    u8 pad4[0xc];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1c[0x14];
    s32 unk30;
    s32 unk34;
} UnkWork_022ed548;

typedef struct UnkTask_022ed548 {
    u8 pad0[8];
    void (*func)(void);
    u8 padC[4];
    UnkWork_022ed548 *work;
} UnkTask_022ed548;

typedef struct UnkTouch_022ed548 {
    u16 unk0;
    u16 y;
    u8 pad4[0xc];
    u32 flags;
} UnkTouch_022ed548;

extern UnkTask_022ed548 *data_022b4574;
extern UnkTouch_022ed548 data_022b65b4;

void func_ov001_022ed490(UnkWork_022ed548 *w, s16 x, s16 y);
void func_ov001_022ed3e0(UnkWork_022ed548 *w);
void func_ov001_022ed390(UnkWork_022ed548 *w, s32 flag);
void func_ov001_022ed19c(UnkWork_022ed548 *w);
void func_ov001_022ed188(UnkWork_022ed548 *w);
void func_ov001_022ed14c(UnkWork_022ed548 *w);
void func_ov001_022ed104(UnkWork_022ed548 *w);
void func_ov001_022ed08c(UnkWork_022ed548 *w);
void func_ov001_022ed6c4(void);

#pragma thumb on
void func_ov001_022ed548(void) {
    UnkWork_022ed548 *w = data_022b4574->work;
    s32 v;
    s32 d;

    func_ov001_022ed490(w, 0, 0x90);
    func_ov001_022ed3e0(w);
    func_ov001_022ed390(w, 0);
    func_ov001_022ed19c(w);
    func_ov001_022ed188(w);
    func_ov001_022ed14c(w);
    func_ov001_022ed104(w);
    func_ov001_022ed08c(w);
    if (data_022b65b4.flags & 0x10) {
        v = (data_022b65b4.y - w->unk10) * 2;
        v += w->unk14;
        if (v < 0)
            v = 0;
        if (v > w->unk34)
            v = w->unk34;
        if (w->unk30 != v) {
            d = v - w->unk30;
            if (w->unk18 == 0) {
                w->unk18 = d;
                w->unk30 = v;
            } else if (w->unk18 > 0) {
                if (d > 0 || d < -2) {
                    w->unk18 = d;
                    w->unk30 = v;
                }
            } else {
                if (d < 0 || d > 2) {
                    w->unk18 = d;
                    w->unk30 = v;
                }
            }
        }
    }
    w->timer++;
    if (!(data_022b65b4.flags & 0x10)) {
        w->timer = 0;
        data_022b4574->func = func_ov001_022ed6c4;
    }
}
#pragma thumb reset
