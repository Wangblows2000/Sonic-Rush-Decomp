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

void NaviMsgFace_StartIdle(UnkWork_020538d8 *w);
void G2x_SetBlendAlpha_(vu32 *p, u32 a, u32 b, u32 c, u32 d);
void NaviMsg_SetVisible(BOOL on);
void NaviMsg_State_Intro(void);

#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

void NaviMsg_StartIntro(void) {
    UnkWork_020538d8 *w;
    u32 plane;

    if (gNaviMsgTask == NULL)
        return;
    w = gNaviMsgTask->work;
    NaviMsgFace_StartIdle(w);
    w->fndc = NaviMsg_State_Intro;
    G2x_SetBlendAlpha_((vu32 *)0x04001050, 2, 4, 0, 0x1f);
    gDisplayTable[1]->blendCnt = 0x442;
    gDisplayTable[1]->blendAlpha = 0x1f00;
    w->unkfc = 0;
    NaviMsg_SetVisible(1);
    plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
    REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane | 6) << 8);
}
