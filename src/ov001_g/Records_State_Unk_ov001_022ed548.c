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

extern UnkTask_022ed548 *gTaskCurrent;
extern UnkTouch_022ed548 gTouchState;

void Records_Unk_ov001_022ed490(UnkWork_022ed548 *w, s16 x, s16 y);
void Records_Unk_ov001_022ed3e0(UnkWork_022ed548 *w);
void Records_Unk_ov001_022ed390(UnkWork_022ed548 *w, s32 flag);
void Records_Unk_ov001_022ed19c(UnkWork_022ed548 *w);
void Records_Unk_ov001_022ed188(UnkWork_022ed548 *w);
void Records_Unk_ov001_022ed14c(UnkWork_022ed548 *w);
void Records_Unk_ov001_022ed104(UnkWork_022ed548 *w);
void Records_Unk_ov001_022ed08c(UnkWork_022ed548 *w);
void Records_State_Unk_ov001_022ed6c4(void);

#pragma thumb on
void Records_State_Unk_ov001_022ed548(void) {
    UnkWork_022ed548 *w = gTaskCurrent->work;
    s32 v;
    s32 d;

    Records_Unk_ov001_022ed490(w, 0, 0x90);
    Records_Unk_ov001_022ed3e0(w);
    Records_Unk_ov001_022ed390(w, 0);
    Records_Unk_ov001_022ed19c(w);
    Records_Unk_ov001_022ed188(w);
    Records_Unk_ov001_022ed14c(w);
    Records_Unk_ov001_022ed104(w);
    Records_Unk_ov001_022ed08c(w);
    if (gTouchState.flags & 0x10) {
        v = (gTouchState.y - w->unk10) * 2;
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
    if (!(gTouchState.flags & 0x10)) {
        w->timer = 0;
        gTaskCurrent->func = Records_State_Unk_ov001_022ed6c4;
    }
}
#pragma thumb reset
