#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0x1994 - 4];
    u8 c[0x1cc4 - 0x1994];
    s32 unk1cc4;
    s32 unk1cc8;
    s32 unk1ccc;
    s32 unk1cd0;
} UnkSub_022e7114;

typedef struct {
    s32 mode;
    s32 unk4;
    s32 unk8;
    u8 padc[0x14 - 0xc];
    s32 state;
    u8 pad18[0x74 - 0x18];
    u32 timer;
    u8 pad78[0xa4 - 0x78];
    UnkSub_022e7114 *sub;
} UnkWork_022e7114;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e7114 *work;
} UnkTask_022e7114;

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022e7114;

extern UnkTask_022e7114 *gTaskCurrent;
extern UnkBright_022e7114 data_0208926c;
extern UnkBright_022e7114 data_02087d44;

BOOL func_ov001_022e2f00(s32 a);
BOOL func_ov001_022e2ee8(s32 a);
void func_ov001_022e3180(s32 a, s32 b);
void func_ov001_022e3840(void *c, s32 a);
void func_ov001_022e3844(void *c, s32 a);
void func_ov001_022e6dec(UnkWork_022e7114 *w);
void SysSound_StopBGM(s32 a);

#pragma thumb on
void func_ov001_022e7114(void) {
    UnkWork_022e7114 *w;
    s32 idx;
    BOOL ok;

    w = gTaskCurrent->work;
    idx = 0;
    switch (w->state) {
    case 1:
        idx = 0;
        break;
    case 2:
        idx = 1;
        break;
    case 3:
        idx = 2;
        break;
    case 6:
        idx = 3;
        break;
    case 8:
        idx = 4;
        break;
    case 12:
        idx = 5;
        break;
    case 13:
        idx = 6;
        break;
    case 14:
        idx = 7;
        break;
    }
    if (w->timer == 0) {
        ok = FALSE;
        switch (w->mode) {
        case 0:
            if (func_ov001_022e2f00(w->unk4)) ok = TRUE;
            break;
        case 1:
            if (func_ov001_022e2ee8(w->unk8)) ok = TRUE;
            break;
        }
        if (ok) {
            func_ov001_022e3180(w->sub->unk1cc4, 0x1c);
        }
        func_ov001_022e3840(w->sub->c, 0x4000);
        func_ov001_022e3844(w->sub->c, 2);
    }
    func_ov001_022e6dec(w);
    w->timer++;
    if (w->timer >= 0x96) {
        data_0208926c.bright = (w->timer - 0x96) >> 1;
        data_02087d44.bright = (w->timer - 0x96) >> 1;
        if (idx == 7) {
            data_0208926c.bright = -data_0208926c.bright;
            data_02087d44.bright = -data_02087d44.bright;
        }
    } else {
        data_0208926c.bright = 0;
        data_02087d44.bright = 0;
    }
    if (w->timer == 0x96) {
        SysSound_StopBGM(0x1f);
    }
    if (w->timer >= 0xb6) {
        w->timer = 0;
        w->sub->unk1cc8 = w->mode;
        w->sub->unk1ccc = idx;
        w->sub->unk1cd0 = 0;
        w->sub->unk0 = 1;
    }
}
#pragma thumb reset
