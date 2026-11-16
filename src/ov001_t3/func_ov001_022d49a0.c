#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSpr_022d49a0;

typedef struct {
    s32 timer;
    UnkSpr_022d49a0 sprB;
    UnkSpr_022d49a0 sprA;
    u32 animB;
    u32 animA;
} UnkWork_022d49a0;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d49a0 *work;
} UnkTask_022d49a0;

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022d49a0;

typedef struct {
    u8 pad0[8];
    u32 flags;
    u16 unkc;
} UnkStruct_022c4680;

typedef struct {
    u8 pad0[0x24];
    u16 unk24;
} UnkStruct_022c46c0;

extern UnkTask_022d49a0 *data_022b4574;
extern UnkStruct_022c4680 data_022c4680;
extern UnkStruct_022c46c0 data_022c46c0;
extern UnkBright_022d49a0 data_0208926c;
extern UnkBright_022d49a0 data_02087d44;

void func_02033550(UnkSpr_022d49a0 *s, u16 id);
void func_0203344c(UnkSpr_022d49a0 *s, s32 a, s32 b);
void func_02032d2c(UnkSpr_022d49a0 *s);
void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void func_02037134(void);
void func_ov001_022d48ec(s32 a, s32 b, s32 c, s32 d);
void func_ov001_022d41e4(void);
void func_ov001_022d4204(void);

#pragma thumb on
void func_ov001_022d49a0(void) {
    UnkWork_022d49a0 *w;
    BOOL flag;
    UnkSpr_022d49a0 *s;
    u32 *a;

    w = data_022b4574->work;
    s = &w->sprB;
    a = &w->animB;
    if (*a != 1) {
        *a = 1;
        func_02033550(s, *a);
    }
    s->flags |= 4;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
    s = &w->sprA;
    a = &w->animA;
    if (*a != 1) {
        *a = 1;
        func_02033550(s, *a);
    }
    s->flags |= 4;
    func_0203344c(s, 0, 0);
    func_02032d2c(s);
    if ((data_022c4680.unkc >= 0x1a || data_022c46c0.unk24 >= 0x1c) && (!(data_022c4680.flags & 0x100) || !(data_022c4680.flags & 0x200))) {
        flag = FALSE;
    } else {
        flag = TRUE;
    }
    if (w->timer == 0x10) {
        if (flag) {
            func_ov001_022d48ec(0, 0x10, 0x10, 0);
        } else {
            func_ov001_022d48ec(0, -0x10, 0x10, 0);
        }
    }
    w->timer++;
    if (w->timer >= 0x20) {
        w->timer = 0;
        if (flag) {
            data_0208926c.bright = 0x10;
            data_02087d44.bright = 0x10;
        } else {
            data_0208926c.bright = -0x10;
            data_02087d44.bright = -0x10;
        }
        GXx_SetMasterBrightness_((vu16 *)0x400006c, data_0208926c.bright);
        GXx_SetMasterBrightness_((vu16 *)0x400106c, data_02087d44.bright);
        func_02037134();
        if (!flag) {
            func_ov001_022d41e4();
            return;
        }
        func_ov001_022d4204();
    }
}
#pragma thumb reset
