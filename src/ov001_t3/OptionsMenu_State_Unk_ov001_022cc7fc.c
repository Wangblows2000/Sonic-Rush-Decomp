#include <nitro/types.h>

typedef struct {
    u32 timer;
    s32 unk4;
    s32 unk8;
} UnkSub_022cc7fc;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    u8 pad20[0x1ec - 0x20];
    UnkSub_022cc7fc sub;
} UnkWork_022cc7fc;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022cc7fc *work;
} UnkTask_022cc7fc;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022cc7fc;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022cc7fc;

extern UnkTask_022cc7fc *gTaskCurrent;
extern UnkPad_022cc7fc gPad;
extern UnkTouch_022cc7fc gTouchState;

void OptionsMenu_UpdateAnims(UnkWork_022cc7fc *w);
void OptionsMenu_DrawItems(UnkWork_022cc7fc *w);
void OptionsMenu_Unk_ov001_022cdef8(void);
void SysSound_PlaySE(s32 a);
BOOL OptionsMenu_Unk_ov001_022c8ba8(u16 x, u16 y);
s32 OptionsMenu_Unk_ov001_022c8d54(UnkSub_022cc7fc *s);
void OptionsMenu_State_Unk_ov001_022cc33c(void);
void OptionsMenu_State_Unk_ov001_022cc6b8(void);

#pragma thumb on
void OptionsMenu_State_Unk_ov001_022cc7fc(void) {
    UnkWork_022cc7fc *w;
    UnkSub_022cc7fc *sub;
    u16 x;
    u16 y;

    w = gTaskCurrent->work;
    sub = &w->sub;
    OptionsMenu_UpdateAnims(w);
    OptionsMenu_DrawItems(w);
    w->sub.timer++;
    w->unk14++;
    w->unk18++;
    if (w->unk1c >= 0) {
        w->unk1c++;
        if (w->unk1c > 10) {
            w->unk1c = -1;
            OptionsMenu_Unk_ov001_022cdef8();
            gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc33c;
        }
    }
    if (w->unk1c < 0) {
        if (gPad.trg & 2) {
            OptionsMenu_Unk_ov001_022cdef8();
            gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc33c;
            SysSound_PlaySE(1);
        } else if (gTouchState.flags & 0x40) {
            x = gTouchState.x;
            y = gTouchState.y;
            if (x < 0x100 && y < 0xc0 && OptionsMenu_Unk_ov001_022c8ba8(x, y)) {
                w->unk1c = 0;
                SysSound_PlaySE(1);
            }
        }
        sub->unk8 = OptionsMenu_Unk_ov001_022c8d54(sub);
        if (sub->unk8 > 0) {
            SysSound_PlaySE(0);
            sub->timer = 0;
            w->unk14 = 0;
            w->unk18 = 0;
            sub->unk4 = 5;
            gTaskCurrent->func = OptionsMenu_State_Unk_ov001_022cc6b8;
        }
    }
}
#pragma thumb reset
