#include <nitro/types.h>

typedef struct {
    u8 pad0[0xdc];
    void (*fndc)(void);
    u8 pade0[0x1c];
    s32 unkfc;
} UnkWork_020538d8;

typedef struct {
    u8 pad0[0x10];
    UnkWork_020538d8 *work;
} UnkTask_020538d8;

typedef struct {
    u8 pad0[0x20];
    u16 blendCnt;
    u16 blendAlpha;
} UnkDisp_020538d8;

extern UnkTask_020538d8 *gNaviMsgTask;
extern UnkDisp_020538d8 *gDisplayTable[];

void func_02052e1c(UnkWork_020538d8 *w);
void func_02003d78(vu32 *p, u32 a, u32 b, u32 c, u32 d);
void func_02053b04(BOOL on);
void func_02052e7c(void);

#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

void func_020538d8(void) {
    UnkWork_020538d8 *w;
    u32 plane;

    if (gNaviMsgTask == NULL)
        return;
    w = gNaviMsgTask->work;
    func_02052e1c(w);
    w->fndc = func_02052e7c;
    func_02003d78((vu32 *)0x04001050, 2, 4, 0, 0x1f);
    gDisplayTable[1]->blendCnt = 0x442;
    gDisplayTable[1]->blendAlpha = 0x1f00;
    w->unkfc = 0;
    func_02053b04(1);
    plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
    REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane | 6) << 8);
}
