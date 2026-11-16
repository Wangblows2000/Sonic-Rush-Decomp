#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 zone;
    u8 act;
} UnkState_0205a728;

typedef struct {
    u8 pad0[0x20];
    u16 unk20;
    u8 pad22[0x26];
    s16 unk48;
} UnkGfx_0205a728;

typedef struct {
    u8 pad0[0x474];
    void (*state)(void *);
    u8 pad478[0x60];
    s16 unk4d8;
    u8 pad4da[0x3f4];
    s16 unk8ce;
} UnkWork_0205a728;

extern UnkState_0205a728 gGameState;
extern UnkGfx_0205a728 gDisplayMain;
extern UnkGfx_0205a728 gDisplaySub;
extern u16 data_02074960[][3][3];
extern u16 data_02074962[][3][3];
extern u16 data_02074964[][3][3];
extern u16 data_02074a04[][3][8];

void StageTitleCard_State_RestoreDisplay(void *w);
u16 StageTitleCard_LerpColor(s32 a, u16 b, s32 c);
void NNS_G3dGlbLightColor(s32 a, u16 b);
void G3X_SetEdgeColorTable(u16 *p);

#define FX_MUL(a, b) ((s32)(((s64)(a) * (b) + 0x800) >> 12))

void StageTitleCard_State_RestoreColors(UnkWork_0205a728 *w) {
    BOOL flag;
    u32 i;
    u16 buf[8];

    w->unk8ce += 0xa3;
    if (w->unk8ce >= 0x1000) {
        w->unk8ce = 0x1000;
        gDisplayMain.unk20 &= ~0xc0;
        w->state = (void (*)(void *))StageTitleCard_State_RestoreDisplay;
    }
    flag = FALSE;
    if (gGameState.zone == 8 && gGameState.act == 2)
        flag = TRUE;
    if (flag) {
        gDisplaySub.unk48 = (FX_MUL(w->unk8ce, -0x10000) + 0x10000) >> 12;
        w->unk4d8 = (FX_MUL(w->unk8ce, -0x1f000) + 0x1f000) >> 12;
    } else if (gGameState.zone >= 9 || gGameState.act >= 2) {
        gDisplaySub.unk48 = (FX_MUL(w->unk8ce, -0x10000) + 0x10000) >> 12;
        gDisplayMain.unk48 = gDisplaySub.unk48;
    } else {
        gDisplaySub.unk48 = (FX_MUL(w->unk8ce, -0x10000) + 0x10000) >> 12;
        w->unk4d8 = (FX_MUL(w->unk8ce, -0x1f000) + 0x1f000) >> 12;
    }
    NNS_G3dGlbLightColor(0, StageTitleCard_LerpColor(0, data_02074960[gGameState.zone][gGameState.act][0], w->unk8ce));
    NNS_G3dGlbLightColor(1, StageTitleCard_LerpColor(0, data_02074962[gGameState.zone][gGameState.act][0], w->unk8ce));
    NNS_G3dGlbLightColor(2, StageTitleCard_LerpColor(0, data_02074964[gGameState.zone][gGameState.act][0], w->unk8ce));
    if (gGameState.zone == 8 && gGameState.act == 2)
        return;
    for (i = 0; i < 8; i++)
        buf[i] = StageTitleCard_LerpColor(0, data_02074a04[gGameState.zone][gGameState.act][i], w->unk8ce);
    G3X_SetEdgeColorTable(buf);
}
