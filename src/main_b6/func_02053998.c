#include <nitro/types.h>

typedef struct {
    s32 mode;
    u8 pad4[0xd0];
    u32 flags;
    u32 padd8;
    void (*fndc)(void);
    u8 pade0[0x1c];
    s32 unkfc;
} UnkWork_02053998;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02053998 *work;
} UnkTask_02053998;

typedef struct {
    s32 unk0;
} UnkState_02053998;

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
    u8 pade[0x56];
    u16 unk64;
} UnkProg_02053998;

typedef struct {
    u8 pad0[0x20];
    u16 blendCnt;
    u16 blendAlpha;
} UnkDisp_02053998;

extern UnkTask_02053998 *gNaviMsgTask;
extern UnkState_02053998 data_022c4560;
extern UnkProg_02053998 gSaveData;
extern UnkDisp_02053998 *gDisplayTable[];

void func_02052e1c(UnkWork_02053998 *w);
void G2x_SetBlendAlpha_(vu32 *p, u32 a, u32 b, u32 c, u32 d);
void func_02053b04(BOOL on);
void func_02053014(void);
void func_02052f4c(void);

void func_02053998(BOOL on) {
    UnkWork_02053998 *w;
    u16 a;
    s16 b;

    if (gNaviMsgTask == NULL)
        return;
    w = gNaviMsgTask->work;
    if (w->mode == 0) {
        if (data_022c4560.unk0 == 0 && gSaveData.unkc < 3)
            return;
        if (data_022c4560.unk0 == 1 && (gSaveData.unk64 < 4 || (gSaveData.unk64 >= 0x1b && gSaveData.unk64 < 0x1c)))
            return;
    }
    if ((w->flags & 1) && on)
        return;
    if (!(w->flags & 1) && !on)
        return;
    w->flags |= 0x800000;
    func_02052e1c(w);
    if (on) {
        a = 0;
        w->fndc = func_02053014;
        b = 0x1f;
    } else {
        a = 0x1f;
        w->fndc = func_02052f4c;
        b = 0;
    }
    G2x_SetBlendAlpha_((vu32 *)0x04001050, 2, 4, a, b);
    gDisplayTable[1]->blendCnt = 0x442;
    gDisplayTable[1]->blendAlpha = a | (b << 8);
    w->unkfc = 0;
    func_02053b04(1);
}
